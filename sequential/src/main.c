#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "graph.h"
#include "array.h"
#include "args.h"

int main(int argc, char* argv[]){
    main_parameters_t c;
    c= get_input(argc, argv);
    FILE* file;
    graph_t *graph;
    array_int *result;

    switch (c.t){
        case INPUT_ERROR:
            printf("incorrent parameters:\n sequential.out filename -> read graph from file named filename\n sequential.out -sm id -> read graph fromm shared memory with identifier id\n");
            exit(1);
            break;
        case INPUT_TYPE_FILE:
            printf("file name:%s, enum INPUT_TYPE_FILE\n", c.name);
            graph = graph_load_from_file(c.name);
            result = (graph_tarjan(graph));
            break;
        case INPUT_TYPE_SHMEM:
            printf("id shared:%s, enum INPUT_TYPE_SHMEM\n", c.name);
            break;
    }

}