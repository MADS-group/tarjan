#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "graph.h"
#include "array.h"
#include "args.h"

void generate_bipartite_graph(long n, char *filename){
    FILE *fp = fopen(filename, "w");
    int tmp[3];
    tmp[0] = (n * 3) + 2;
    tmp[2] = -1;
    fwrite(tmp, sizeof(int), 1, fp);
    fwrite(&n, sizeof(int), 1, fp);
    for(long i=0; i<n; i++){
        tmp[0] = i*2;
        tmp[1] = i*2+1; 
        fwrite(tmp, sizeof(int), 3, fp);
    }
    fclose(fp);
}

int main(int argc, char* argv[]){
    
    if(argc == 4){

        graph_t * t;
        char output[200];
        long n_node;
        int mode;
        
        sscanf(argv[1],"%s",output);
        sscanf(argv[2],"%ld", &n_node);
        sscanf(argv[3],"%d", &mode);

        printf("%s , %ld, %d\n", output,n_node,mode);
        if(mode == 0 || mode == 1){
            t=graph_fully_connected_disconnected(n_node,mode);
            graph_save_to_file(t, output);
            graph_free(t);
        } else if (mode == 2){
            generate_bipartite_graph(n_node, output);
        }
        
        //graph_print_debug(t);
        
        return 0;
    }else{
        printf("inserire come  primo parametro path file contente il grafo di uscita,\nsecondo parametro numero nodi da generare,\nterzo parametro 0 per grafo completamente disconesso, 1 per grafo completamente connesso, 2 per grafo bipartito\n");
        return 1;
    }
    
}