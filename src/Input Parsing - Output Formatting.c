#define ui unsigned int

void expect_graph(const ui N, ui *adj_matrix) {
    ui i, j, weight;

    for(i = 0; i < N; i ++) {
        for(j = 0; j < N; j ++) {
            weight = expect_int();

            adj_matrix[(i * N) + j] = weight;
        }
    }
}

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

