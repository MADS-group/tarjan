#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "graph.h"
#include "array.h"
#include "args.h"
#include <time.h>

int main(int argc, char* argv[]){
    srand(time(NULL));
    
    if(argc == 5){

        char output[200];
        int n_node;
        int mean_edges;
        double variance_edges;
        
        sscanf(argv[1],"%s",output);
        sscanf(argv[2],"%d", &n_node);
        sscanf(argv[3],"%d", &mean_edges);
        sscanf(argv[4],"%lf", &variance_edges);

        printf("%s , %d, %d, %f\n", output,n_node,mean_edges,variance_edges);

        graph_t * t;
        t=graph_random(n_node, mean_edges, variance_edges);
        //graph_random(1000, 2, 1);
        graph_save_to_file(t, output);
        graph_free(t);

    }else{
        printf("inserire come  primo parametro path file contente il grafo di uscita,\nsecondo parametro numero nodi da generare,\nterzo parametro media archi da generare per ogni nodo,\nquarto parametro varianza archi da generare per ogni nodo.\n");
        return 1;

    }
    
}