#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#define ui unsigned int
#define ulli unsigned long long int

typedef struct t_node{
    ui   index;
    ulli priority;   
} node;

typedef struct min_heap{
    ui capacity;
    ui size;
    node *heap;
} min_priority_queue;

void init_min_pq(const ui capacity, min_priority_queue *min_pq) {
    min_pq->capacity = capacity;
    min_pq->size = 0;
    min_pq->heap = (node *)malloc(min_pq->capacity * sizeof(node));
}

void resize_min_pq(min_priority_queue *min_pq) {
    ui i;

    min_pq->capacity *= 2;
    node * tmp = (node *)malloc(min_pq->capacity * sizeof(node));
     
    for(i = 0; i < min_pq->size; i ++) {
        tmp[i] = min_pq->heap[i];
    }

    free(min_pq->heap);
    min_pq->heap = tmp;
}

void destroy_min_pq(min_priority_queue *min_pq) {
    free(min_pq->heap);
}

ui is_empty_min_pq(const min_priority_queue *min_pq) {
    return min_pq->size == 0;
}

ui peek_min_pq(const min_priority_queue *min_pq) {
    return min_pq->heap[0].index;
}

ui sink_min_pq(const ui i, min_priority_queue *min_pq) {
    ui position, left, right;

    position = i;
    left = i * 2 + 1;
    right = i * 2 + 2;

    if(left < min_pq->size && min_pq->heap[left].priority < min_pq->heap[position].priority) {
        position = left;
    }

    if(right < min_pq->size && min_pq->heap[right].priority < min_pq->heap[position].priority) {
        position = right;
    }

    if(position != i) {
        node tmp;

        tmp = min_pq->heap[position];
        min_pq->heap[position] = min_pq->heap[i];
        min_pq->heap[i] = tmp;
    }

    return position;
}

ui swim_min_pq(const ui i, min_priority_queue *min_pq) {
    ui parent;

    if(i == 0) {
        return i;
    }
    parent = (i - 1) / 2;

    if(min_pq->heap[parent].priority > min_pq->heap[i].priority) {
        node tmp;

        tmp = min_pq->heap[parent];
        min_pq->heap[parent] = min_pq->heap[i];
        min_pq->heap[i] = tmp;
        
        return parent;
    }

    return i;
}

void push_min_pq(const ui index, const ui priority, min_priority_queue *min_pq) {
    int from, to;

    if(min_pq->size == min_pq->capacity) {
        resize_min_pq(min_pq);
    }

    min_pq->heap[min_pq->size].index = index;
    min_pq->heap[min_pq->size].priority = priority;

    to = min_pq->size;
    min_pq->size ++;

    do {
        from = to;
        to = swim_min_pq(from, min_pq);
    } while(from != to);
}

void pop_min_pq(min_priority_queue *min_pq) {
    int from, to;

    if(is_empty_min_pq(min_pq)) {
        return;
    }

    min_pq->size --;
    min_pq->heap[0] = min_pq->heap[min_pq->size];

    if(min_pq->size <= 1) {
        return;
    }

    to = 0;

    do {
        from = to;
        to = sink_min_pq(from, min_pq);
    } while(from != to);
}