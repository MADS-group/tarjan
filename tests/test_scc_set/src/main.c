#include <stdlib.h>
#include <stdio.h>
#include "graph.h"

KHASH_MAP_INIT_INT(m32, int)
int main(int argc, char* argv[]){
    (void) argc;
    (void) argv;
    scc_set_t *SCCs = scc_set_init();

    array_int *scc = array_int_init(10);
    array_int_push(scc, 10);
    array_int_push(scc, 9);
    array_int_push(scc, 8);
    array_int_push(scc, 7);
    array_int_push(scc, 6);
    array_int_push(scc, 5);
    
    printf("%d",array_int_get_min(scc));

    //scc_set_add(SCCs, array_int_get_min(scc), scc);
    scc_set_print_debug(SCCs);

    array_int_free(scc);
    scc_set_free(SCCs);
}