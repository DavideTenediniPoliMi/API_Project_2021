#include <stdlib.h>
#include <stdio.h>
#include "Heap.c"

#define ULLIMAX 9223372036854775808ULL
#define ui unsigned int
#define ulli unsigned long long int

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
