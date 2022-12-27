#include <stdio.h>
#include <stdio.h>
#include "bitarray.h"

// kernel uses 10 registers
__global__ void DeleteTrivialSCCs(int* adj_lists, int* adj_list_indexes, int n_vertices, int *bitmask, bool *terminate) {
    int vertex_id = blockDim.x * blockIdx.x + threadIdx.x;
    
    //If thread is not associated with a vertex or the vertex has already been eliminated then do nothing
    //printf("vertex: %d vertex_id: %d n_vertices: %d\n", vertex_id, vertex_id, n_vertices);
    if(vertex_id >= n_vertices){
        return;
    }

    if(test_bit(bitmask, vertex_id) != 0){ 
        //printf("vertex: %d has already been eliminated\n", vertex_id);
        return;
    }
    int adj_list_start = adj_list_indexes[vertex_id];
    int adj_list_end = adj_list_indexes[vertex_id+1];
    int elim = false;
    //printf("vertex: %d adj_list_start: %d adj_list_end: %d\n", vertex_id, adj_list_start, adj_list_end);
    //If adjacency list is empty or has a self-loop delete the vertex
    if(adj_list_start == adj_list_end){
        elim = true;
    }
    if(((adj_list_start + 1) == adj_list_end) && adj_lists[adj_list_start] == vertex_id){
        elim = true;
    }

    if(elim == true){
        //printf("Sto eliminando1\n");
        (*terminate) = false;
        set_bit(bitmask, vertex_id);
        //printf("vertex: %d eliminated1\n", vertex_id);
        return;
    }

    //If vertex has no incoming edges delete the vertex
    elim = true;
    //printf("adj_list_indexes[n_vertices]%d\n",adj_list_indexes[n_vertices]);
    for(int i = 0; i < adj_list_indexes[n_vertices]; i++){
        if(adj_lists[i] == vertex_id){ //! Manca una condizione
            //printf("vertex: %d found\n", vertex_id);
            elim = false;
            break;
        }
    }

    if(elim == true){
        //printf("vertex: %d eliminated2\n", vertex_id);
        (*terminate) = false;
        set_bit(bitmask, vertex_id);
    }
}
