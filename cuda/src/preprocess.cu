#include <stdio.h>
#include <stdio.h>
#include "bitarray.h"

__global__ void MatrixMulKernel(int* adj_lists, int* adj_list_indexes, int *bitmask, int n_vertexes) {
    int vertex_id = blockDim.x * blockIdx.x + threadIdx.x;
    //If thread is not associated with a vertex or the vertex has already been eliminated then do nothing
    if(vertex_id < n_vertexes || test_bit(bitmask, vertex_id)){ 
        return;
    }
    int adj_list_start = adj_list_indexes[vertex_id];
    int adj_list_end = adj_list_indexes[vertex_id+1];
    int elim = false;
    //If adjacency list is empty or has a self-loop delete the vertex
    if(adj_list_start == adj_list_end){
        elim = true;
    }
    if(((adj_list_start + 1) == adj_list_end) && adj_lists[adj_list_start] == vertex_id){
        elim = true;
    }

    if(elim == true){
        //delete
        return;
    }

    //If vertex has no incoming edges delete the vertex
    elim = true;
    for(int i = 0; i < adj_list_indexes[n_vertexes]; i++){
        if(adj_lists[i] == vertex_id){
            elim = false;
            break;
        }
    }

    if(elim == true){
        //delete
    }
}