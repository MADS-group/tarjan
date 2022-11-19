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
    for(int i = 0; i < 100; i++){
        graph_insert_edge(graph, i, i+1);
    }
    graph_insert_edge(graph, 49, 0);
    graph_insert_edge(graph, 100, 50);
    graph_insert_edge(graph, 50, 49);
    array_int *scc = graph_tarjan(graph);
    array_int_print(scc);
    graph_free(graph);
}

void test_serialize(){
    graph_t *graph = graph_init();
    for(int i = 0; i <= 5; i++)
        graph_insert_vertex(graph,i);
    for(int i = 0; i < 4; i++){
        graph_insert_edge(graph, 5, i);
        graph_insert_edge(graph, i, 5);
    }
    khint_t pos = 0;
    array_int *serial1 = graph_serialize(graph,2,&pos);
    array_int *serial2 = graph_serialize(graph,2,&pos);
    array_int *serial3 = graph_serialize(graph,2,&pos);
    array_int *serial4 = graph_serialize(graph,2,&pos);
    array_int *serial5 = graph_serialize(graph,200,&pos);
    array_int_print(serial1);
    array_int_print(serial2);
    array_int_print(serial3);
    array_int_print(serial4);
    array_int_print(serial5);
    graph_free(graph);
}

void test_deserialize(){
    graph_t *graph = graph_init();
    array_int *buff;
    graph_deserialize(graph,buff);
}

int main(int argc, char* argv[]){
    test_init_destroy();
    test_serialize();
    test_tarjan();
    printf("All tests passed successfully\n");
}