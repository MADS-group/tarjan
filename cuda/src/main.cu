#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include "cuda_graph.h"
#include "preprocess.cu"
#include "array.h"
#include "graph.h"

#define THREADxBLOCK 1024

using namespace std;

scc_set_t *SCCs;

void callback(array_int * scc){
    int scc_id;
    scc_id = array_int_get_min(scc);
    scc_set_add(SCCs,scc_id,scc);
}

int main(int argc, char **argv){
    char path_inputfilename[100];
    char output_filename[100];
    int *bitmask, *d_bitmask;
    int *d_adj_lists, *d_adj_list_indexes;
    bool terminate = false;
    bool *d_terminate;

    if(argc != 3 ){
        printf("Error! Wrong or missing parameters. Please run the program specifing the path of the graph to compute and the name the output file.\n");
        exit(1);
    }
    sscanf(argv[1],"%s",path_inputfilename);
    sscanf(argv[2],"%s",output_filename);

    cuda_graph_t *cuda_graph = cuda_graph_load_from_file(path_inputfilename);
    int n_vertices = cuda_graph->n_vertex;
    int n_bitmask = ((n_vertices-1)/32)+1;
    bitmask = new int[n_bitmask](); //Instantiate an array and initialize it to 0
    //Eseguire il kernel
    int gridsize = ((n_vertices-1)/THREADxBLOCK) + 1; //Numero blocchi su una dimensione della griglia
    dim3 dimGrid(gridsize);
    dim3 dimBlock(THREADxBLOCK);

    cudaMalloc(&d_adj_lists, cuda_graph->adj_lists_len * sizeof(int));
    cudaMemcpy(d_adj_lists, cuda_graph->adj_lists, cuda_graph->adj_lists_len * sizeof(int), cudaMemcpyHostToDevice);
    cudaMalloc(&d_adj_list_indexes, (n_vertices + 1) * sizeof(int));
    cudaMemcpy(d_adj_list_indexes, cuda_graph->adj_list_indexes, (n_vertices + 1) * sizeof(int), cudaMemcpyHostToDevice);

    cudaMalloc(&d_bitmask, n_bitmask * sizeof(int));
    cudaMemset(d_bitmask, 0, n_bitmask * sizeof(int));

    cudaMalloc(&d_terminate, sizeof(bool));
    int iter = 0;
    while(!terminate){
        //printf di C++
        cout << "Called " << iter++ << " times" <<endl;
        terminate = true;
        cudaMemset(d_terminate, true, sizeof(bool));
        printf("start del kernel\n");
        DeleteTrivialSCCs<<<dimGrid, dimBlock>>>(d_adj_lists, d_adj_list_indexes, n_vertices, d_bitmask, d_terminate);
        cudaDeviceSynchronize();
        cudaMemcpy(bitmask, d_bitmask, n_bitmask * sizeof(int), cudaMemcpyDeviceToHost);
        /*for(int i = 0; i < n_bitmask; i++){
            printf("%x ", bitmask[i]);
        }
        printf("\n");*/
        //device to host
        cudaMemcpy(&terminate, d_terminate, sizeof(bool), cudaMemcpyDeviceToHost);
    }
    cudaMemcpy(bitmask, d_bitmask, n_bitmask * sizeof(int), cudaMemcpyDeviceToHost);
    
    //printf("Bitmask: %x\n", bitmask[0]);
    //First
    //00000000.00000000.00000000.00011000 <-- 31-0
    //00000000.00000000.00000000.00010001 <-- 63-32
    //00000000.00000000.00000000.00000101 <-- 95-64
    //00000000.00000000.00000000.00000101 <-- 127-96
    //Second
    //00000000.00000000.00100001.00011000 <-- 31-0
    //00000000.00000000.00000100.00010011 <-- 63-32
    //00000000.00000000.00000000.00010111 <-- 95-64
    //00000000.00000000.00000000.00000111 <-- 127-96
    //Third
    //00000000.10000000.00100011.00011000 <-- 31-0
    //00000000.00001000.00000100.00010111 <-- 63-32
    //00000000.00000000.00000000.10011111 <-- 95-64
    //00000000.00000000.00000000.00000111 <-- 127-96

    //Lanciare tarjan sequenziale
    //STARTTIME(1);
    graph_t* graph = cuda_graph_to_graph(cuda_graph, bitmask);
    graph_print_debug(graph);
    cuda_graph_free(cuda_graph);
    SCCs = scc_set_init();
    //printf("start graph\n");
    //graph_print_debug(graph);
    //ENDTIME(1,time_init);

    //STARTTIME(2);
    graph_tarjan_foreach(graph, callback);
    //ENDTIME(2,time_tarjan);

    //num = graph_get_num_vertex(graph);
    //STARTTIME(3);
    scc_set_save_to_file(SCCs,output_filename);
    cudaFree(d_adj_list_indexes);
    cudaFree(d_adj_lists);
    cudaFree(d_bitmask);
    cudaFree(d_terminate);
    graph_free(graph);
    scc_set_free(SCCs);
    delete[] bitmask;
    //ENDTIME(3,time_destroy);
    //printf("%d,%f,%f,%f,",num,time_init,time_destroy,time_tarjan);

    
    return 0;
}