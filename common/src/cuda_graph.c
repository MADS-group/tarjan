#include <cuda_graph.h>
#include <bitarray.h>
#include <stdio.h>

cuda_graph_t *cuda_graph_load_from_file(char *filename){
    FILE *fp = fopen(filename, "r");
    if (fp == NULL){
       printf("Error opening file %s, aborting.\n", filename);   // Program exits if the file pointer returns NULL.
       return NULL;
    }
    int n;
    fread(&n, sizeof(int), 1, fp);
    array_int *array = array_int_init(n+1);
    array_int_set(array, 0, n); //Fill position 0
    array_int_resize(array, n+1); //Sets array length = n+1
    fread(array_int_get_ptr(array)+1, sizeof(int), n, fp); //Fill from position 1 to n
    cuda_graph_t *G = NULL;

    fclose(fp);
    array_int_free(array);
    return G;
}

graph_t *cuda_graph_to_graph(cuda_graph_t *G, int *deleted_bitarray){
    graph_t *result_G = graph_init();
    int src_vertex, target_vertex;
    for(src_vertex = 0; src_vertex < G->n_vertex; src_vertex++){
        if(test_bit(deleted_bitarray, src_vertex)){
            continue;
        }
        graph_insert_vertex(result_G, src_vertex);
        for(int i = G->adj_list_indexes[src_vertex]; i<G->adj_list_indexes[src_vertex+1]; i++){
            target_vertex = G->adj_lists[i];
            if(test_bit(deleted_bitarray, target_vertex)){
                continue;
            }
            graph_insert_vertex(result_G, target_vertex);
            graph_insert_edge(result_G, src_vertex, target_vertex);
        }   
    }
    return result_G;
}

void cuda_graph_free(cuda_graph_t *G){

}