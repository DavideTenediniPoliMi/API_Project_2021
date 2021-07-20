#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#define ULLIMAX 9223372036854775808ULL
#define STARTK 10000
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
        return EOF;
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

    return ch;
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

// Heap
typedef struct t_node{
    ui   index;
    ulli priority;   
} node;

typedef struct heap{
    ui bound;
    ui capacity;
    ui size;
    int (*cmp_fun)(const node *, const node *);
    node *heap;
} priority_queue;

int cmp_fun_min(const node *n1, const node *n2) {
    return (n2->priority - n1->priority);
}

int cmp_fun_max(const node *n1, const node *n2) {
    return (n1->priority != n2->priority) ? (n1->priority - n2->priority) : -1;
}

ui min_ui(ui n1, ui n2) {
    return (n1 < n2) ? n1 : n2;
}

void init_pq(const ui bound, const ui capacity, int (*cmp_fun)(const node *, const node *), priority_queue *pq) {
    pq->bound = bound;
    pq->capacity = capacity;
    pq->size = 0;
    pq->cmp_fun = cmp_fun;
    pq->heap = (node *)malloc(pq->capacity * sizeof(node));
}

void destroy_pq(priority_queue *pq) {
    free(pq->heap);
}

void resize_pq(priority_queue *pq) {
    node *tmp;
    ui i;

    if(pq->size == pq->bound) {
        return;
    }

    pq->capacity = min_ui(pq->capacity * 2, pq->bound);
    tmp = (node *)malloc(pq->capacity * sizeof(node));

    for(i = 0; i < pq->size; i ++) {
        tmp[i] = pq->heap[i];
    }

    destroy_pq(pq);
    pq->heap = tmp;
}

void reset_pq(priority_queue *pq) {
    pq->size = 0;
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

    if(left < pq->size && pq->cmp_fun(&pq->heap[left], &pq->heap[position]) > 0) {
        position = left;
    }

    if(right < pq->size && pq->cmp_fun(&pq->heap[right], &pq->heap[position]) > 0) {
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

    if(pq->cmp_fun(&pq->heap[parent], &pq->heap[i]) < 0) {
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

    if(pq->size == pq->capacity) {
        resize_pq(pq);
    }

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

    if(bounded_pq->size < bounded_pq->bound) {
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

// Graph Scoring
void find_shortest_paths(const ui N, const ui *adj_matrix, ulli *distances, priority_queue *min_pq, ui* pushed) {
    ui i;

    reset_pq(min_pq);

    distances[0] = 0;
    for(i = 1; i < N; i ++) {
        distances[i] = ULLIMAX;
    }

    pushed[0] = 1;
    for(i = 1; i < N; i ++) {
        pushed[i] = 0;
    }

    push_pq(0, 0, min_pq);

    while(!is_empty_pq(min_pq)) {
        ui curr_node, weight;
        curr_node = peek_pq(min_pq);
        pop_pq(min_pq);
        pushed[curr_node] = 0;

        for(i = 0; i < N; i ++) {
            weight = adj_matrix[(curr_node * N) + i];
            
            if(weight != 0 && distances[i] > distances[curr_node] + weight) {
                distances[i] = distances[curr_node] + weight;
                if(!pushed[i]) {
                    pushed[i] = 1;
                    push_pq(i, distances[i], min_pq);
                }
            }
        }
    }

    
}

ulli compute_score(const ui N, const ui* adj_matrix, ulli *distances, priority_queue *min_pq, ui *pushed) {
    ulli score;
    ui i;

    find_shortest_paths(N, adj_matrix, distances, min_pq, pushed);

    score = 0;
    for(i = 0; i < N; i ++) {
        if(distances[i] != ULLIMAX) {
            score += distances[i];  
        }
    }

    return score;
}

// Program Flow
void add_graph(const ui N, const ui index, priority_queue *bounded_pq, ui *adj_matrix, ulli *distances, priority_queue *min_pq, ui *pushed) {
    ulli score;

    expect_graph(N, adj_matrix);

    score = compute_score(N, adj_matrix, distances, min_pq, pushed);

    push_bounded_pq(index, score, bounded_pq);
}

// Output Formatting
void print_topK(priority_queue *bounded_pq) {
    ui i;

    if(bounded_pq->size == 0) {
        pc('\n');
        return;
    }

    for(i = 0; i < bounded_pq->size - 1; i ++) {
        print_int(bounded_pq->heap[i].index);
        pc(' ');
    }

    print_int(bounded_pq->heap[bounded_pq->size - 1].index);
    pc('\n');
}

int main() {
    priority_queue *bounded_pq;
    priority_queue *min_pq;
    ui *pushed;
    ui *adj_matrix;
    ulli *distances;
    ui N, K, index;
    char ch;

    index = 0;
    N = expect_int();
    K = expect_int();
    
    bounded_pq = (priority_queue *)malloc(sizeof(priority_queue));
    min_pq = (priority_queue *)malloc(sizeof(priority_queue));
    pushed = (ui *)malloc(N * sizeof(ui));
    adj_matrix = (ui *)malloc(N * N * sizeof(ui));
    distances = (ulli *)malloc(N * sizeof(ulli));

    init_pq(K, min_ui(K , STARTK), cmp_fun_max, bounded_pq);
    init_pq(N, N / 2, cmp_fun_min, min_pq);

    while((ch = expect_char()) != EOF) { 
        if(ch == 'A') {
            add_graph(N, index, bounded_pq, adj_matrix, distances, min_pq, pushed);
            index ++;
        } else {            
            print_topK(bounded_pq);

            do{
                ch = gc();
            }while(ch != '\n' && ch != EOF);
        }
    }

    destroy_pq(bounded_pq);
    destroy_pq(min_pq);
    free(min_pq);
    free(bounded_pq);
    free(pushed);
    free(adj_matrix);
    free(distances);
    return 0;
}