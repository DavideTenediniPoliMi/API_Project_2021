#include "Heap.c"

#define ui unsigned int
#define ulli unsigned long long int
#define STARTK 10000

void expect_graph(const ui N, ui *adj_matrix) {
    ui i, j, weight;

    for(i = 0; i < N; i ++) {
        for(j = 0; j < N; j ++) {
            weight = expect_int();
            
            adj_matrix[(i * N) + j] = weight;
        }
    }
}

void add_graph(const ui N, const ui index, priority_queue *bounded_pq, ui *adj_matrix, ulli *distances, priority_queue *min_pq, ui *pushed) {
    ulli score;

    expect_graph(N, adj_matrix);

    score = compute_score(N, adj_matrix, distances, min_pq, pushed);

    push_bounded_pq(index, score, bounded_pq);
}


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