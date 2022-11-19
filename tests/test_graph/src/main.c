#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
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

void test_tarjan2(){
    graph_t *graph = graph_init();
    char test[]="9 8 -1 7 -1 6 5 4 -1 3 2 1 0 -1 10 -1 \n";
    char s1[41];
    memset(s1,0,41);
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

    array_int *scc = graph_tarjan(graph);
    array_int_sprint(scc,s1);
    assert(strcmp(test,s1)==0);
    graph_free(graph);
}

void test_tarjan_withoutEdge(){
    graph_t *graph = graph_init();
    char test[]="0 -1 1 -1 2 -1 3 -1 4 -1 5 -1 6 -1 7 -1 8 -1 9 -1 10 -1 11 -1 12 -1 13 -1 14 -1 15 -1 16 -1 17 -1 18 -1 19 -1 20 -1 21 -1 22 -1 23 -1 24 -1 25 -1 26 -1 27 -1 28 -1 29 -1 30 -1 31 -1 32 -1 33 -1 34 -1 35 -1 36 -1 37 -1 38 -1 39 -1 40 -1 41 -1 42 -1 43 -1 44 -1 45 -1 46 -1 47 -1 48 -1 49 -1 50 -1 51 -1 52 -1 53 -1 54 -1 55 -1 56 -1 57 -1 58 -1 59 -1 60 -1 61 -1 62 -1 63 -1 64 -1 65 -1 66 -1 67 -1 68 -1 69 -1 70 -1 71 -1 72 -1 73 -1 74 -1 75 -1 76 -1 77 -1 78 -1 79 -1 80 -1 81 -1 82 -1 83 -1 84 -1 85 -1 86 -1 87 -1 88 -1 89 -1 90 -1 91 -1 92 -1 93 -1 94 -1 95 -1 96 -1 97 -1 98 -1 99 -1 100 -1 \n";
    char *s1;
    s1=(char *)malloc(601*sizeof(char));
    memset(s1,0,601);
    for(int i = 0; i <= 100; i++)
        graph_insert_vertex(graph,i);
    array_int *scc = graph_tarjan(graph);
    //array_int_print(scc);
    array_int_sprint(scc,s1);
    //printf("%s",s1);
    assert(strcmp(test,s1)==0);
    free(s1);
    graph_free(graph);
}

void test_tarjan_vertxAllConnect(){
    graph_t *graph = graph_init();
    char test[]="100 99 98 97 96 95 94 93 92 91 90 89 88 87 86 85 84 83 82 81 80 79 78 77 76 75 74 73 72 71 70 69 68 67 66 65 64 63 62 61 60 59 58 57 56 55 54 53 52 51 50 49 48 47 46 45 44 43 42 41 40 39 38 37 36 35 34 33 32 31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0 -1 \n";
    char *s1;
    s1=(char *)malloc(301*sizeof(char));
    memset(s1,0,301);
    for(int i = 0; i <= 100; i++)
        graph_insert_vertex(graph,i);
    for(int i = 0; i <= 100; i++){
        for(int k=0; k<=100; k++)
            graph_insert_edge(graph, i, k);
    }
    array_int *scc = graph_tarjan(graph);
    array_int_sprint(scc,s1);
    //printf("%s",s1);
    assert(strcmp(test,s1)==0);
    free(s1);
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
    //test_init_destroy();
    //test_serialize();
    test_tarjan2();
    test_tarjan_withoutEdge();
    test_tarjan_vertxAllConnect();
    printf("All tests passed successfully\n");
}