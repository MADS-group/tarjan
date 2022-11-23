#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "graph.h"
#include "array.h"
#include "args.h"

int main(int argc, char* argv[]){
    graph_t * t, *c;
    t=graph_random(5, 2, 1);
    printf("\n\ngrafo t:\n");
    graph_print_debug(t);
    c=graph_random(5, 2, 1);
    printf("\n\ngrafo c:\n");
    graph_print_debug(c);

    graph_merge(t, c, 0.5);
    printf("\n\nnuovo grafo t:\n");
    graph_print_debug(t);

    graph_free(t);
    graph_free(c);
}