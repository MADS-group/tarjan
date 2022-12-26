#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "graph.h"
#include "array.h"
#include "cuda_graph.h"
#include "bitarray.h"
int main(int argc, char* argv[]){
    if(argc != 2){
        fprintf(stderr, "Invalid parameters.\nUsage: %s [graph]\n", argv[0]);
        exit(1);
    }
    cuda_graph_t *G;
    G = cuda_graph_load_from_file(argv[1]);
    cuda_graph_print_debug(G);
    int *mask = (int *) malloc( sizeof(int) * ( ( (G->n_vertex - 1) / 8 ) + 1 ) );
    memset(mask, 0, sizeof(int) * ( ( (G->n_vertex - 1) / 8 ) + 1 ));
    graph_t *G2 = cuda_graph_to_graph(G, mask);
    graph_print_debug(G2);
    cuda_graph_free(G);
    graph_free(G2);
    free(mask);
    return 0;
}