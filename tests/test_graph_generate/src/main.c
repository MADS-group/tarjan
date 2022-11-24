#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "graph.h"
#include "array.h"
#include "args.h"

int main(int argc, char* argv[]){
    if(argc <= 1){
        graph_t * t, *c;
        t=graph_random(10, 2, 1);
        printf("\n\ngrafo t:\n");
        graph_print_debug(t);
        c=graph_copy(t);
        printf("\n\ngrafo c:\n");
        graph_print_debug(c);

        graph_merge(t, c, 0.2);
        printf("\n\nnuovo grafo t:\n");
        graph_print_debug(t);

        printf("\n\ngrafo c:\n");
        graph_print_debug(c);

        graph_free(t);
        graph_free(c);
    } else if(argc == 2){
        int n=0;
        int i=0;
        sscanf(argv[1],"%d", &n);
        printf("creo %d grado di partenza\n", n);

        graph_t * t, *c;
        t=graph_random(10, 2, 1);
        printf("\n\ngrafo t:\n");
        graph_print_debug(t);
        c=graph_copy(t);
        printf("\n\ngrafo c:\n");
        graph_print_debug(c);

        for(i=0; i<n; i++){
            graph_merge(t, c, 0.2);
        }
        printf("\n\nnuovo grafo t:\n");
        graph_print_debug(t);

        printf("\n\ngrafo c:\n");
        graph_print_debug(c);

        graph_free(t);
        graph_free(c);

    }else{
        printf("cosa cazzo hai messo in ingresso cornuto\n");
    }
    
}