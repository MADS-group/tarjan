#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "graph.h"
#include "array.h"
#include "cuda_graph.h"

int main(int argc, char* argv[]){
    if(argc != 2){
        fprintf(stderr, "Invalid parameters.\nUsage: %s [graph]\n", argv[0]);
        exit(1);
    }
    cuda_graph_t *G;
    G = cuda_graph_load_from_file(argv[1]);
    cuda_graph_print_debug(G);
    cuda_graph_free(G);
    return 0;
}