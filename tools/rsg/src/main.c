#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "graph.h"
#include "array.h"
#include "args.h"

int main(int argc, char* argv[]){
    srand(42);
    
    if(argc == 2){
        char path[200];
    
        sscanf(argv[1],"%s",path);

        graph_t * t, *c;
        t=graph_random(10, 2, 1);
        printf("seed generato t:\n");
        graph_print_debug(t);
        graph_save_to_file(t, path);
        c=graph_load_from_file(path);
        printf("seed letto c:\n");
        graph_print_debug(c);
    }else{
        printf("inserire come parametro path file contente il grafo di uscita \n");
        return 1;

    }
    
}
//"./../../data/seed.bin"
///leggo seed da file e genero 