#include <stdio.h>
#include <stdlib.h>
#include "cuda_graph.h"
#include "array.h"
#include "graph.h"

scc_set_t *SCCs;

void callback(array_int * scc){
    int scc_id;
    scc_id = array_int_get_min(scc);
    scc_set_add(SCCs,scc_id,scc);
}

int main(int argc, char **argv){

    char path_inputfilename[100];
    char output_filename[100];
    int *bitmask;
    if(argc != 3 ){
        printf("Error! Wrong or missing parameters. Please run the program specifing the path of the graph to compute and the name the output file.\n");
        exit(1);
    }
    sscanf(argv[1],"%s",path_inputfilename);
    sscanf(argv[2],"%s",output_filename);

    cuda_graph_t * cuda_graph = cuda_graph_load_from_file(path_inputfilename);
    //Eseguire il kernel

    

    //Lanciare tarjan sequenziale
    //STARTTIME(1);
    graph_t* graph = cuda_graph_to_graph(cuda_graph, bitmask);
    cuda_graph_free(cuda_graph);
    SCCs= scc_set_init();
    //printf("start graph\n");
    //graph_print_debug(graph);
    //ENDTIME(1,time_init);

    //STARTTIME(2);
    graph_tarjan_foreach(graph, callback);
    //ENDTIME(2,time_tarjan);

    //num = graph_get_num_vertex(graph);
    //STARTTIME(3);
    scc_set_save_to_file(SCCs,output_filename);
    graph_free(graph);
    scc_set_free(SCCs);
    
    //ENDTIME(3,time_destroy);
    //printf("%d,%f,%f,%f,",num,time_init,time_destroy,time_tarjan);

    
    return 0;
}