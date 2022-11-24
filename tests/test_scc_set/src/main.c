#include <stdlib.h>
#include <stdio.h>
#include "graph.h"

void pippo(){
    //    (void) argc;
//    (void) argv;
    scc_set_t *SCCs = scc_set_init();

    array_int *scc = array_int_init(10);
    array_int_push(scc, 10);
    array_int_push(scc, 9);
    array_int_push(scc, 8);
    array_int_push(scc, 7);
    array_int_push(scc, 6);
    array_int_push(scc, 5);
    scc_set_add(SCCs, array_int_get_min(scc), scc);
    scc_set_print_debug(SCCs);
    array_int_free(scc);

    printf("\n");
    scc = array_int_init(10);
    array_int_push(scc, 1);
    array_int_push(scc, 2);
    array_int_push(scc, 3);
    array_int_push(scc, 4);
    array_int_push(scc, 99);
    array_int_push(scc, 88);
    scc_set_add(SCCs, array_int_get_min(scc), scc);
    scc_set_print_debug(SCCs);
    array_int_free(scc);

    printf("\n");
    scc = array_int_init(10);
    array_int_push(scc, 7);
    array_int_push(scc, 155);
    scc_set_add(SCCs, array_int_get_min(scc), scc);
    scc_set_print_debug(SCCs);
    array_int_free(scc);

    printf("\n");
    scc = array_int_init(10);
    array_int_push(scc, 99);
    array_int_push(scc, 0);
    scc_set_add(SCCs, array_int_get_min(scc), scc);
    scc_set_print_debug(SCCs);
    array_int_free(scc);


    scc_set_free(SCCs);
}

void test_scc_set_add(){
    scc_set_t *SCCs = scc_set_init();
    array_int *scc = array_int_init(10);
    
    array_int_push(scc, 10);
    array_int_push(scc, 9);
    array_int_push(scc, 8);
    array_int_push(scc, 7);
    array_int_push(scc, 6);
    array_int_push(scc, 5);
    scc_set_add(SCCs, array_int_get_min(scc), scc);
    scc_set_print_debug(SCCs);
    array_int_free(scc);

    printf("\nAggiungo un nuovo scc:\n");
    scc = array_int_init(10);
    array_int_push(scc, 1);
    array_int_push(scc, 2);
    array_int_push(scc, 3);
    array_int_push(scc, 4);
    array_int_push(scc, 99);
    array_int_push(scc, 8);
    array_int_push(scc, 88);
    scc_set_add(SCCs, array_int_get_min(scc), scc);
    scc_set_print_debug(SCCs);
    array_int_free(scc);

    scc_set_free(SCCs);
}

void test_tarjan_scc(){
    scc_set_t *SCCs = scc_set_init();
    graph_t *graph = graph_init();

    for(int i = 0; i <= 10; i++)
        graph_insert_vertex(graph,i);
    
    graph_insert_edge(graph, 0, 1);
    graph_insert_edge(graph, 0, 3);
    graph_insert_edge(graph, 1, 2);
    graph_insert_edge(graph, 1, 4);
    graph_insert_edge(graph, 2, 0);
    graph_insert_edge(graph, 2, 6);
    graph_insert_edge(graph, 3, 2);
    graph_insert_edge(graph, 4, 5);
    graph_insert_edge(graph, 4, 6);
    graph_insert_edge(graph, 5, 6);
    graph_insert_edge(graph, 5, 7);
    graph_insert_edge(graph, 5, 8);
    graph_insert_edge(graph, 5, 9);
    graph_insert_edge(graph, 6, 4);
    graph_insert_edge(graph, 7, 9);
    graph_insert_edge(graph, 8, 9);
    graph_insert_edge(graph, 9, 8);


    graph_free(graph);
    scc_set_free(SCCs);
}

//KHASH_MAP_INIT_INT(m32, int)
int main(int argc, char* argv[]){
//    (void) argc;
//    (void) argv;
    test_scc_set_add();
    printf("All tests passed successfully\n");
}