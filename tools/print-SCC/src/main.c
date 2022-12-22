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
    scc_set_t *S = scc_set_load_from_file(argv[1]);
    scc_set_print_debug(S);
    scc_set_free(S);
    return 0;
}
