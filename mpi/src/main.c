#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include <mpi.h>

int main(int argc,char* argv[]){
    int rank, size;
    MPI_Init(&argc,&argv);
    MPI_Init(NULL,NULL);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Status status;

    if(size <= 1){
        //Se il numero di processi è 1 allora non posso eseguire il programma in maniera parallela
        printf("Not enough process!\n");
        exit(1);
    }

    //Convenzioni: il processo con rank 0 è il nodo master
    //i processi con rank diverso da 0 sono nodi slave
    int v_graph;
    if(rank == 0){
        //codice del master
        char *filename = "graph";
        graph_t* graph;
        graph = graph_load_from_file(filename);
        v_graph = graph_get_num_vertex(graph);

        
    }

    if(rank != 0){
        //codice degli slave

    }
        



    



    MPI_Finalize();
    return 0;
}