#include <stdlib.h>
#include <stdio.h>
#include "graph.h"
#include "array.h"

void test_init_destroy(){
    graph_t *graph = graph_init();
    graph_free(graph);
}

void test_tarjan(){
    graph_t *graph = graph_init();
    for(int i = 0; i <= 100; i++)
        graph_insert_vertex(graph,i);
    for(int i = 0; i < 50; i++){
        graph_insert_edge(graph, i, i+1);
        graph_insert_edge(graph, i+50, i+51);
    }
    graph_insert_edge(graph, 49, 0);
    graph_insert_edge(graph, 100, 50);
    array_int *scc = graph_tarjan(graph);
    array_int_print(scc);
    graph_free(graph);
}

int main(int argc, char* argv[]){
    test_init_destroy();
    test_tarjan();
    printf("All tests passed successfully\n");
}