#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include "measurement.h"
#include <mpi.h>
#include <sys/resource.h>

int main(int argc,char* argv[]){
    int rank, size;
    struct rlimit rl;
    getrlimit(RLIMIT_STACK, &rl);
    rl.rlim_cur=128000000;
    setrlimit(RLIMIT_STACK, &rl);
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    
    char path[100];
    char outputfilename[50];

    if(argc != 3 ){
        printf("Error! Wrong or missing parameters. Please run the program specifing the path of the graph to compute and the name the output file.\n");
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }


    sscanf(argv[1],"%s",path);
    sscanf(argv[2],"%s",outputfilename);

    if(size <= 1){
        //Se il numero di processi è 1 allora non posso eseguire il programma in maniera parallela
        printf("This application is meant to be run with at least 2 processes.\n");
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    //Convenzioni: il processo con rank 0 è il nodo master
    //I processi con rank diverso da 0 sono nodi slave

    if(rank == 0){
        master_work(rank,size,path,outputfilename);

        //printf("\nFINITO DI FARE TUTTO.\n");
        //MPI_Abort(MPI_COMM_WORLD, MPI_SUCCESS);
    }

    if(rank != 0){
        //codice degli slave
        //printf("Sono lo slave %d\n",rank);
        slave_work(rank);
    }

    MPI_Finalize();
    return 0;
}