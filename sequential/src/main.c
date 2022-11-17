#include <stdlib.h>
#include <stdio.h>
#include "graph.h"
#include "array.h"

int main(int argc, char* argv[]){
    FILE* file;
    file = fopen("test.txt", "r");
    // TODO: Aggiungere controllo degli arguments
    //Devono essere supportati:
    //sequential.out nomefile -> legge da nomefile il grafo
    //sequential.out -sm id -> legge da shared memory il grafo
    //In ogni altro caso deve spiegare come si usa il programma. Fare tutto in maniera modulare in una libreria common/
    //perché servirà per altri programmi.
    
    if (NULL == file) {
        printf("file can't be opened \n");
    }

    graph_t *graph;
    graph = graph_init(3); //aggiungere n
    graph = graph_read_from_file(file);

    array_int *result;
    result = (graph_tarjan(graph));
}