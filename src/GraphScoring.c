#include <stdlib.h>
#include <stdio.h>
#include "Min-Heap.c"

#define ui unsigned int
#define ulli unsigned long long int
#define INTMAX 4294967295

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