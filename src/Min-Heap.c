#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#define ui unsigned int
#define ulli unsigned long long int

typedef struct t_node{
    ui   index;
    ulli priority;   
} node;

typedef struct heap{
    ui capacity;
    ui size;
    ui (*cmp_fun)(const node *, const node *);
    node *heap;
} priority_queue;

ui cmp_fun_min(const node *n1, const node *n2) {
    return (n1->priority < n2->priority);
}

ui cmp_fun_max(const node *n1, const node *n2) {
    return (n1->priority > n2->priority);
}

void init_pq(const ui capacity, ui (*cmp_fun)(const node *, const node *), priority_queue *pq) {
    pq->capacity = capacity;
    pq->size = 0;
    pq->cmp_fun = cmp_fun;
    pq->heap = (node *)malloc(pq->capacity * sizeof(node));
}

void destroy_pq(priority_queue *pq) {
    free(pq->heap);
}

ui is_empty_pq(const priority_queue *pq) {
    return pq->size == 0;
}

ui peek_pq(const priority_queue *pq) {
    return pq->heap[0].index;
}

ui sink_pq(const ui i, priority_queue *pq) {
    ui position, left, right;

    position = i;
    left = i * 2 + 1;
    right = i * 2 + 2;

    if(left < pq->size && pq->cmp_fun(&pq->heap[left], &pq->heap[position])) {
        position = left;
    }

    if(right < pq->size && pq->cmp_fun(&pq->heap[right], &pq->heap[position])) {
        position = right;
    }

    if(position != i) {
        node tmp;

        tmp = pq->heap[position];
        pq->heap[position] = pq->heap[i];
        pq->heap[i] = tmp;
    }

    return position;
}

ui swim_pq(const ui i, priority_queue *pq) {
    ui parent;

    if(i == 0) {
        return i;
    }
    parent = (i - 1) / 2;

    if(!pq->cmp_fun(&pq->heap[parent], &pq->heap[i])) {
        node tmp;

        tmp = pq->heap[parent];
        pq->heap[parent] = pq->heap[i];
        pq->heap[i] = tmp;
        
        return parent;
    }

    return i;
}

void push_pq(const ui index, const ui priority, priority_queue *pq) {
    ui from, to;

    pq->heap[pq->size].index = index;
    pq->heap[pq->size].priority = priority;

    to = pq->size;
    pq->size ++;

    do {
        from = to;
        to = swim_pq(from, pq);
    } while(from != to);
}

void push_bounded_pq(const ui index, const ui priority, priority_queue *bounded_pq) {
    ui from, to;

    if(bounded_pq->size < bounded_pq->capacity) {
        push_pq(index, priority, bounded_pq);
        return;
    }

    if(bounded_pq->heap[0].priority <= priority) {
        return;
    }

    to = 0;
    bounded_pq->heap[0].index = index;
    bounded_pq->heap[0].priority = priority;

    do {
        from = to;
        to = sink_pq(from, bounded_pq);
    } while (from != to);
}

void pop_pq(priority_queue *pq) {
    int from, to;

    if(is_empty_pq(pq)) {
        return;
    }

    pq->size --;
    pq->heap[0] = pq->heap[pq->size];

    if(pq->size <= 1) {
        return;
    }

    to = 0;

    do {
        from = to;
        to = sink_pq(from, pq);
    } while(from != to);
}