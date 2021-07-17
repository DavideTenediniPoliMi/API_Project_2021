#include <stdlib.h>
#include <stdio.h>

#define ui unsigned int
#define ulli unsigned long long int

typedef struct t_node{
    ui   index;
    ulli priority;   
} node;

typedef struct bounded_max_heap {
    ui bound;
    ui size;
    node *heap;
} bounded_max_priority_queue;

void init_b_max_pq(const ui bound, bounded_max_priority_queue * b_max_pq) {
    b_max_pq->bound = bound;
    b_max_pq->size = 0;
    b_max_pq->heap = (node *)malloc(bound * sizeof(node));
}

void destroy_b_max_pq(bounded_max_priority_queue * b_max_pq) {
    free(b_max_pq->heap);
}

ui is_empty_b_max_pq(const bounded_max_priority_queue *b_max_pq) {
    return b_max_pq->size == 0;
}

ui sink_b_max_pq(ui i, bounded_max_priority_queue *b_max_pq) {
    ui position, left, right;

    position = i;
    left = i * 2 + 1;
    right = i * 2 + 2;

    if(left < b_max_pq->size && b_max_pq->heap[left].priority > b_max_pq->heap[position].priority) {
        position = left;
    }

    if(right < b_max_pq->size && b_max_pq->heap[right].priority > b_max_pq->heap[position].priority) {
        position = right;
    }

    if(position != i) {
        node tmp;

        tmp = b_max_pq->heap[position];
        b_max_pq->heap[position] = b_max_pq->heap[i];
        b_max_pq->heap[i] = tmp;
    }

    return position;
}

ui swim_b_max_pq(const ui i, bounded_max_priority_queue *b_max_pq) {
    ui parent;

    if(i == 0) {
        return i;
    }
    parent = (i - 1) / 2;

    if(b_max_pq->heap[parent].priority <= b_max_pq->heap[i].priority) {
        node tmp;

        tmp = b_max_pq->heap[parent];
        b_max_pq->heap[parent] = b_max_pq->heap[i];
        b_max_pq->heap[i] = tmp;
        
        return parent;
    }

    return i;
}

void push_b_max_pq(const ui index, const ui priority, bounded_max_priority_queue *b_max_pq) {
    ui from, to;

    if(b_max_pq->size == b_max_pq->bound) {
        if(b_max_pq->heap[0].priority <= priority) {
            return;
        }

        to = 0;
        b_max_pq->heap[0].index = index;
        b_max_pq->heap[0].priority = priority;

        do {
            from = to;
            to = sink_b_max_pq(from, b_max_pq);
        } while (from != to);

        return;
    }

    b_max_pq->heap[b_max_pq->size].index = index;
    b_max_pq->heap[b_max_pq->size].priority = priority;
    to = b_max_pq->size;
    b_max_pq->size ++;

    do {
        from = to;
        to = swim_b_max_pq(from, b_max_pq);
    } while(from != to);
}