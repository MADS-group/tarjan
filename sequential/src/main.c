#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "graph.h"
#include "array.h"
#include "args.h"

/*! @function
  @abstract      call sequential tarjan on graph from file or from shared memory
  @param  argc   number of parameter, considering the 1st is name of program
  @param  argv   in case of graph from file the second parameters is the path of grapf file, in case
                graph is from shared memory the second paramenters is ID of shared memory
 */
//vdsdfv
int main(int argc, char* argv[]){
    main_parameters_t c;
    c= get_input(argc, argv);
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
            graph_print_debug(graph);
            result = (graph_tarjan(graph));
            array_int_print(result);
            graph_free(graph);
            array_int_free(result);
            break;
        case INPUT_TYPE_SHMEM:
            printf("id shared:%s, enum INPUT_TYPE_SHMEM\n", c.name);
            break;
    }

}