#include <stdlib.h>
#include <stdio.h>

#define ui unsigned int

typedef struct t_node{
    ui index;
    ui value;   
}node;

typedef struct min_heap{
    ui capacity;
    ui size;
    node *heap;
}min_priority_queue;

void init_min_heap(const ui capacity, min_priority_queue *min_pq) {
    min_pq->capacity = capacity;
    min_pq->size = 0;
    min_pq->heap = (node *)malloc(min_pq->capacity * sizeof(node));
}

void resize(min_priority_queue *min_pq) {
    ui i;

    min_pq->capacity *= 2;
    node * tmp = (node *)malloc(min_pq->capacity * sizeof(node));
     
    for(i = 0; i < min_pq->size; i ++) {
        tmp[i] = min_pq->heap[i];
    }

    free(min_pq->heap);
    min_pq->heap = tmp;
}

void destroy_min_heap(min_priority_queue *min_pq) {
    free(min_pq->heap);
}

node* peek(const min_priority_queue *min_pq) {
    if(min_pq->size == 0) {
        return NULL;
    }

    return &(min_pq->heap[0]);
}

ui sink(const ui i, min_priority_queue *min_pq) {
    ui position, left, right;

    position = i;
    left = i * 2 + 1;
    right = i * 2 + 2;

    if(left < min_pq->size && min_pq->heap[left].value < min_pq->heap[position].value) {
        position = left;
    }

    if(right < min_pq->size && min_pq->heap[right].value < min_pq->heap[position].value) {
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

ui swim(const ui i, min_priority_queue *min_pq) {
    ui parent;

    if(i == 0) {
        return i;
    }
    parent = (i - 1) / 2;

    if(min_pq->heap[parent].value > min_pq->heap[i].value) {
        node tmp;

        tmp = min_pq->heap[parent];
        min_pq->heap[parent] = min_pq->heap[i];
        min_pq->heap[i] = tmp;
        
        return parent;
    }

    return i;
}

void push(const ui index, const ui value, min_priority_queue *min_pq) {
    int from, to;

    if(min_pq->size == min_pq->capacity) {
        resize(min_pq);
    }

    min_pq->heap[min_pq->size].index = index;
    min_pq->heap[min_pq->size].value = value;

    to = min_pq->size;
    min_pq->size ++;

    do {
        from = to;
        to = swim(from, min_pq);
    } while(from != to);
}

void pop(min_priority_queue *min_pq) {
    if(min_pq->size == 0) {
        return;
    }

    min_pq->size --;
    min_pq->heap[0] = min_pq->heap[min_pq->size];

    if(min_pq->size <= 1) {
        return;
    }

    sink(0, min_pq);
}

int main() {
    min_priority_queue * mpq = (min_priority_queue *)malloc(sizeof(min_priority_queue));
    node *n;

    init_min_heap(5, mpq);

    push(0, 5, mpq);
    n = peek(mpq);
    printf("%d %d\n", n->index, n->value);
    push(1, 7, mpq);
    n = peek(mpq);
    printf("%d %d\n", n->index, n->value);
    push(2, 1, mpq);
    n = peek(mpq);
    printf("%d %d\n", n->index, n->value);
    push(3, 1, mpq);
    n = peek(mpq);
    printf("%d %d\n", n->index, n->value);
    push(4, 5, mpq);
    n = peek(mpq);
    printf("%d %d\n", n->index, n->value);
    push(5, 324, mpq);
    n = peek(mpq);
    printf("%d %d\n", n->index, n->value);

    destroy_min_heap(mpq);
    free (mpq);
}