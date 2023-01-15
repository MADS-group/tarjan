#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "graph.h"
#include "cuda_graph.h"
#include "bitarray.h"
#include "array.h"
#include "args.h"
#include "measurement.h"
#include <sys/resource.h>
/*! @function
  @abstract      call sequential tarjan on graph from file or from shared memory
  @param  argc   number of parameter, considering the 1st is name of program
  @param  argv   in case of graph from file the second parameters is the path of grapf file, in case
                graph is from shared memory the second paramenters is ID of shared memory
 */

scc_set_t *SCCs;

void callback(array_int * scc){
    int scc_id;
    scc_id = array_int_get_min(scc);
    scc_set_add(SCCs,scc_id,scc);
}

void preprocess(cuda_graph_t *cuda_graph, int *bitmask, int n_vertices){
    bool stop = false, elim;
    int adj_list_start;
    int adj_list_end;
    while(!stop){
        stop = true;
        for(int vertex_id = 0; vertex_id < n_vertices; vertex_id++){ // Foreach vertex in the graph
            if(test_bit(bitmask, vertex_id) != 0){ // Skip the vertex if it has already been eliminated
                continue;
            }
            adj_list_start = cuda_graph->adj_list_indexes[vertex_id]; // Adj list start
            adj_list_end = cuda_graph->adj_list_indexes[vertex_id+1]; // Adj list end

            elim = false;

            // Delete the vertex if it has no outgoing edges
            if(adj_list_start == adj_list_end){ 
                elim = true; // Mark the vertex as "to be deleted"
            }
            
            // Delete the vertex if it has only a self loop
            if(((adj_list_start + 1) == adj_list_end) && cuda_graph->adj_lists[adj_list_start] == vertex_id){
                elim = true; // Mark the vertex as "to be deleted"
            }

            if(elim){
                stop = false;
                set_bit(bitmask, vertex_id); // Mark the vertex as deleted
                continue; // Skip to the next vertex
            }

            // Delete the vertex if it has no incoming edges
            elim = true;
            for(int i = 0; i < cuda_graph->adj_list_indexes[n_vertices]; i++){ //adj_list_indexes[n_vertices] is the length of adj_lists[] array
                if(cuda_graph->adj_lists[i] == vertex_id){
                    elim = false;
                    break;
                }
            }

            if(elim){
                stop = false;
                set_bit(bitmask, vertex_id); // Mark the vertex as deleted
                continue; // Skip to the next vertex
            }
        }
    }
}

int main(int argc, char* argv[]){
    main_parameters_t c;
    graph_t *graph;
    cuda_graph_t *cuda_graph;
    int n_vertices_after_preprocess, n_vertices_before_preprocess, n_bitmask;
    int *bitmask;
    struct rlimit rl;
    double time_tarjan = 0.0, time_init = 0.0, time_destroy = 0.0, time_preprocess = 0.0;

    //STACK SETUP
    getrlimit(RLIMIT_STACK, &rl);
    rl.rlim_cur=128000000;
    setrlimit(RLIMIT_STACK, &rl);
    c = get_input(argc, argv);
    
    switch (c.t){
        case INPUT_ERROR:
            printf("incorrent parameters:\ninserire come primo parametro path del grafo ingresso\ninserire come secondo parametro path del file contenente gli SCC trovati\n");
            exit(1);
            break;
        case INPUT_TYPE_FILE:
            {
            STARTTIME(1);
            cuda_graph_t *cuda_graph = cuda_graph_load_from_file(c.first_param);
            n_vertices_before_preprocess = cuda_graph->n_vertex;
            n_bitmask = ((n_vertices_before_preprocess-1)/32)+1;
            bitmask = (int *) malloc(sizeof(int) * n_bitmask); //Instantiate an array and initialize it to 0
            SCCs= scc_set_init();
            ENDTIME(1,time_init);
            STARTTIME(2);
            preprocess(cuda_graph, bitmask, n_vertices_before_preprocess);
            graph = cuda_graph_to_graph(cuda_graph, bitmask);
            ENDTIME(2,time_preprocess);

            STARTTIME(3);
            graph_tarjan_foreach(graph, callback);
            ENDTIME(3,time_tarjan);

            n_vertices_after_preprocess = graph_get_num_vertex(graph);
            STARTTIME(4);
            scc_set_save_to_file(SCCs,c.second_param);
            graph_free(graph);
            cuda_graph_free(cuda_graph);
            scc_set_free(SCCs);
            free(bitmask);
            ENDTIME(4,time_destroy);

            //Output format:
            //num vertex before preprocess, num vertex after preprocess, init time, destory time, tarjan time, preprocess time
            printf("%d,%d,%f,%f,%f,%f,", n_vertices_before_preprocess, n_vertices_after_preprocess, time_init, time_destroy, time_tarjan, time_preprocess); 
            break;
            }
    }

}

