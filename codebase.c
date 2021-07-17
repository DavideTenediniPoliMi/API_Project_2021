#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <time.h>

#define ui unsigned int

#ifdef  _WIN32
#define gc _getchar_nolock
#define pc _putchar_nolock
#endif

#ifdef  linux
#define gc getchar_unlocked
#define pc putchar_unlocked
#endif

// Fast IO
int expect_int() {
    char ch;
    ui num;

    do {
        ch = gc();
    } while (ch != EOF && !isdigit(ch));

    if(ch == EOF) {
        return -1;
    }

    num = 0;
    do {
        num = num * 10 + (ch - '0');
        ch = gc();
    } while(isdigit(ch));

    return num;
}

int expect_char() {
    char ch;

    do {
        ch = gc();
    } while(ch != EOF && !isalpha(ch));

    return (ch == EOF) ? -1 : ch;
}

void print_int(const ui n) {
    ui trail0, reversed;
    ui num;
    num = n;

    if(num == 0) {
        pc('0');
        return;
    }

    trail0 = 0;
    while(num % 10 == 0) {
        trail0 ++;
        num /= 10;
    }

    reversed = 0;
    while(num >= 10) {
        reversed = (reversed * 10) + (num % 10);
        num /= 10;
    }

    if(num != 0) {
        pc(num + '0');
    }
        
    while(reversed != 0) {
        pc((reversed % 10) + '0');
        reversed /= 10;
    }

    while(trail0 > 0) {
        pc('0');
        trail0 --;
    }
}

// Input Parsing
void expect_graph(const ui N, ui *adj_matrix) {
    ui i, j, weight;

    for(i = 0; i < N; i ++) {
        for(j = 0; j < N; j ++) {
            weight = expect_int();

            adj_matrix[(i * N) + j] = weight;
        }
    }
}

// Min-Heap
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

// Implicit Heap // Bounded Min-Heap

// Graph Scoring

// Program Flow
void add_graph(const ui N) {
    ui *adj_matrix = (ui *)malloc(N * N * sizeof(ui));
    
    expect_graph(N, adj_matrix);
    
    //calculate graph score

    //update topk 

    free(adj_matrix);
}

// Output Formatting
void print_topK(const ui K, const ui *topK) {
    ui i;

    if(K == 0) {
        pc('\n');
        return;
    }

    for(i = 0; i < K - 1; i ++) {
        print_int(topK[i]);
        pc(' ');
    }

    print_int(topK[K - 1]);
    pc('\n');
}

int main() {
    ui N, K;
    char ch;

    N = expect_int();
    K = expect_int();

    while((ch = expect_char()) != -1) { 
        if(ch == 'A') {
            add_graph(N);
        } else {
            //print_topK(K);
            print_int(K);
            while(ch != -1 && ch != '\n') {
                ch = expect_char();
            }
        }
    }
    
    return 0;
}