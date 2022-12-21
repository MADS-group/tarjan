#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "graph.h"
#include "array.h"
#include "args.h"

int main(int argc, char* argv[]){
    
    if(argc == 4){

        graph_t * t;
        char output[200];
        int n_node;
        int isFullyConnected;
        
        sscanf(argv[1],"%s",output);
        sscanf(argv[2],"%d", &n_node);
        sscanf(argv[3],"%d", &isFullyConnected);

        printf("%s , %d, %d\n", output,n_node,isFullyConnected);
        
        t=graph_fully_connected_disconnected(n_node,isFullyConnected);
        //graph_print_debug(t);
        graph_save_to_file(t, output);
        graph_free(t);
        return 0;
    }else{
        printf("inserire come  primo parametro path file contente il grafo di uscita,\nsecondo parametro numero nodi da generare,\nterzo parametro 0 per grafo completamente disconesso, 1 per grafo completamente connesso\n");
        return 1;
    }
    
}