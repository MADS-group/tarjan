#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "graph.h"
#include "array.h"
#include "args.h"

int main(int argc, char* argv[]){
    
    if(argc == 3){
        char first[200];
        char second[200];
    
        sscanf(argv[1],"%s",first);
        sscanf(argv[2],"%s", second);

        scc_set_t * result1=scc_set_init();
        scc_set_t * result2=scc_set_init();

        result1= scc_set_load_from_file(first);
        result2= scc_set_load_from_file(second);

        if(scc_set_contains(result1, result2) && scc_set_contains(result2, result1)){
            printf("correct output\n");
        }else{
            printf("incorrect output\n");
        }
        scc_set_free(result1);
        scc_set_free(result2);
    }else{
        printf("incorrect param");
    }
               
}
