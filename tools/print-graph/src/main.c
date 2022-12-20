#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "graph.h"
#include "array.h"
#include "args.h"

int main(int argc, char* argv[]){
    if(argc != 2){
        printf("Usage: %s [input_file]\n", argv[0]);
        return 1;
    }
    graph_t *G = graph_load_from_file(argv[1]);
    graph_print_debug(G);
    graph_free(G);
    return 0;
}
