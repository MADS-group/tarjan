#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "graph.h"
#include "array.h"
#include "args.h"

int main(int argc, char* argv[]){

    if(argc == 3){
        int n=0;
        int i=0;
        char path[200];
    
        sscanf(argv[1],"%s",path);
        sscanf(argv[2],"%d", &n);
        if(n<0){
            printf("inserire secondo parametro maggiore o uguale di 0\n");
            return 1;
        }

        graph_t * seed, *c;
        seed=graph_load_from_file(path);
        // il seed è in "./../../data/seed.bin"
        printf("seed letto:\n");
        graph_print_debug(seed);

        for(i=0; i<n; i++){
            c=graph_copy(seed);
            graph_merge(seed, c, 0.2);
            graph_free(c);
        }
        graph_save_to_file(seed, "./../../data/graph_generated.bin");
        printf("\n\nnuovo grafo:\n");
        graph_print_debug(seed);

        graph_free(seed);
    }else{
        printf("inserire come primo paramentro path del grafo seed (graph.bin)\ncome secondo parametro il numero di moltiplicazioni di tale seed\n es: seed 10 nodi e secondo paramentro 1 grafo generato 20 nodi\n es: seed 10 nodi e secondo paramentro 2 grafo generato 40 nodi\n es: seed 10 nodi e secondo paramentro 3 grafo generato 80 nodi\n es: seed 10 nodi e secondo paramentro 4 grafo generato 160 nodi\n");
        return 1;

    }
               
}
