#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
//#include <inttypes.h>
#include <time.h>

#define INTMAX 4294967295
#define ui unsigned int
#define ulli unsigned long long int

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
    ui trail0, reversed, num;
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

void push_min_pq(const ui index, const ulli priority, min_priority_queue *min_pq) {
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

// Implicit Heap // Bounded Min-Heap
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

// Graph Scoring
void find_shortest_paths(const ui N, const ui *adj_matrix, ulli *distances) {
    min_priority_queue *min_pq;
    ui i;

    min_pq = (min_priority_queue *)malloc(sizeof(min_priority_queue));
    init_min_pq(N, min_pq);

    distances[0] = 0;
    for(i = 1; i < N; i ++) {
        distances[i] = INTMAX;
    }

    push_min_pq(0, 0, min_pq);

    while(!is_empty_min_pq(min_pq)) {
        ui curr_node, weight;
        curr_node = peek_min_pq(min_pq);
        pop_min_pq(min_pq);

        for (i = 0; i < N; i ++) {
            weight = adj_matrix[(curr_node * N) + i];
            
            if(weight != 0 && distances[i] > distances[curr_node] + weight) {
                distances[i] = distances[curr_node] + weight;
                push_min_pq(i, distances[i], min_pq);
            }
        }
    }

    destroy_min_pq(min_pq);
    free(min_pq);
}

ulli compute_score(const ui N, const ui* adj_matrix) {
    ulli *distances;
    ulli score;
    ui i;

    distances = (ulli *)malloc(N * sizeof(ulli));

    find_shortest_paths(N, adj_matrix, distances);

    score = 0;
    for(i = 0; i < N; i ++) {
        if(distances[i] != INTMAX) {
            score += distances[i];  
        }
    }

    free(distances);
    return score;
}

// Program Flow
void add_graph(const ui N, const ui index, bounded_max_priority_queue *b_max_pq) {
    ui *adj_matrix = (ui *)malloc(N * N * sizeof(ui));  
    ulli score;

    expect_graph(N, adj_matrix);

    score = compute_score(N, adj_matrix);

    push_b_max_pq(index, score, b_max_pq);

    free(adj_matrix);
}

// Output Formatting
void print_topK(bounded_max_priority_queue *b_max_pq) {
    ui i;

    if(b_max_pq->size == 0) {
        pc('\n');
        return;
    }

    for(i = 0; i < b_max_pq->size - 1; i ++) {
        print_int(b_max_pq->heap[i].index);
        pc(' ');
    }

    print_int(b_max_pq->heap[b_max_pq->size - 1].index);
    pc('\n');
}

int main() {
    bounded_max_priority_queue *b_max_pq;
    ui N, K, index;
    char ch;

    index = 0;
    N = expect_int();
    K = expect_int();
    b_max_pq = (bounded_max_priority_queue *)malloc(sizeof(bounded_max_priority_queue));
    init_b_max_pq(K, b_max_pq);

    while((ch = expect_char()) != -1) { 
        if(ch == 'A') {
            add_graph(N, index, b_max_pq);
            index ++;
        } else {            
            print_topK(b_max_pq);

            gc();
            gc();
            gc();
            gc();
        }
    }

    destroy_b_max_pq(b_max_pq);
    free(b_max_pq);
    return 0;
}