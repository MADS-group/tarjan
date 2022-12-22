#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "graph.h"
#include "array.h"
#include "args.h"

int main(int argc, char* argv[]){
    srand(42);
    
    if(argc == 5){
        int n=0;
        int i=0;
        int p=0;
        char path[200];
        char output[200];
    
        sscanf(argv[1],"%s",path);
        sscanf(argv[2],"%s", output);
        sscanf(argv[3],"%d", &n);
        sscanf(argv[4],"%d", &p);
        if(n<0){
            printf("inserire secondo parametro maggiore o uguale di 0\n");
            return 1;
        }

        graph_t * seed, *c;
        seed=graph_load_from_file(path);
        // il seed è in "./../../data/seed.bin"

        for(i=0; i<n; i++){
            c=graph_copy(seed);
            graph_merge(seed, c, p);
            graph_free(c);
        }
        graph_save_to_file(seed, output);

        graph_free(seed);
    }else{
        printf("inserire come primo paramentro path del grafo seed (graph.bin)\ninserire come secondo paramentro path del grafo uscita (out.bin)\ninserire come terzo parametro il numero di moltiplicazioni di tale seed\n  es: seed 10 nodi e secondo paramentro 1 grafo generato 20 nodi\n  es: seed 10 nodi e secondo paramentro 2 grafo generato 40 nodi\n  es: seed 10 nodi e secondo paramentro 3 grafo generato 80 nodi\n  es: seed 10 nodi e secondo paramentro 4 grafo generato 160 nodi\ninserire come quarto parametro probabilità di creare un arco tra un nodo del seed di partenza e il nuovo grafo aggiunto\n");
        return 1;

    }
               
}
