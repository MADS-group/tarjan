#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include <mpi.h>
#include <assert.h>
#include <random.h>

typedef enum {
    MPI_TAG_DATA = 500, //0
    MPI_TAG_SERVICE = 501, //1
    MPI_TAG_SIZE = 502 //2
} mpi_tag_t;
//Gli enum sono mappati a 0 e 1. Se questi numero non hanno messaggi speciali per MPI.

int main(int argc,char* argv[]){
    int rank, size;
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    
    if(size <= 1){
        //Se il numero di processi è 1 allora non posso eseguire il programma in maniera parallela
        printf("This application is meant to be run with at least 2 processes.\n");
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    //Convenzioni: il processo con rank 0 è il nodo master
    //i processi con rank diverso da 0 sono nodi slave
    if(rank == 0){
        printf("[MASTER] Master rank is %d\n",rank);
        master_work(rank,size);
    }

    if(rank != 0){
        //codice degli slave
        printf("[SLAVE] Slave rank is %d\n",rank);
        slave_work();
    }

    MPI_Finalize();
    return 0;
}


void master_work(int rank,int size){
    
    int dim;
    int done;
    int flag_size, flag_service;
    int *array;
    MPI_Status status_dim,status_data,status_service;
    MPI_Request request_dim,request_data,request_service;

    while(1){
        flag_size = 0;
        flag_service = 0;

        MPI_Irecv(&dim,1, MPI_INT,MPI_ANY_SOURCE,500,MPI_COMM_WORLD,&request_dim);
        //Se il secondo test è andato a buon fine -> devo decrementare still_working
        MPI_Irecv(&done,1,MPI_INT,MPI_ANY_SOURCE,501,MPI_COMM_WORLD,&request_service);

        printf("[Master] Waiting the slave sending something...\n");
        while(!flag_service && !flag_size){ //Se non ci sono stati nuovi messaggi e c'è almeno uno slave ancora in esecuzione            
            MPI_Test(&request_dim, &flag_size, &status_dim);
            //MPI_Test(&request_service, &flag_service, &status_service);
        }
        printf("[Master] Slave sent something!!!\n");
        //IF DEBUG
        if(flag_size){
            printf("[MASTER] I received a size: %d with status: status_dim.MPI_SOURCE: %d status_dim.MPI_TAG: %d status_dim.MPI_ERROR: %d \n", dim, status_dim.MPI_SOURCE, status_dim.MPI_TAG, status_dim.MPI_ERROR);
        }

        if(flag_size){ //Ho trovato uno slave che ha finito. Ora devo lavorare sull'scc appena calcolato dallo slave 
            //Devo essere sicuro di prendere l'scc del nodo che ha fatto uscire dal while loop precendente con status_data.MPI_SOURCE.
            array = malloc(dim*sizeof(int));
            assert(array != NULL);

            MPI_Irecv(array,dim,MPI_INT,status_dim.MPI_SOURCE,501,MPI_COMM_WORLD,&request_data);
            printf("[MASTER] Irevc called\n");
            MPI_Wait(&request_data,&status_data);
            printf("[MASTER] Received a SCC with status: status_data.MPI_SOURCE: %d status_data.MPI_TAG: %d status_data.MPI_ERROR: %d \n", dim, status_data.MPI_SOURCE, status_data.MPI_TAG, status_data.MPI_ERROR);
            printf("[MASTER] printing scc with length %d...\n",dim);
            for(int i =0;i<dim;i++){
                printf("a[%d]=%d ",i,array[i]);
            }
            printf("\n");

        }
        
        if(flag_service){
            printf("Closing everything\n");
            exit(1);
        }
        





    }


}


void slave_work(){

    srand(420);
    int dim;
    int *array;

    MPI_Status status_dim, status_data;
    MPI_Request request_dim, request_data;


    while(1){
        dim = rand() % 15 + 1;
        array = malloc(dim*sizeof(int));
        assert(array != NULL);

        for(int i = 0;i<dim;i++){
            array[i] = rand() % 100 +1;
            printf("array[%d]=%d\n", i, array[i]);
        }
        

        printf("[SLAVE]: Sending array size: %d\n",dim);
        MPI_Send(&dim,1,MPI_INT,0,500,MPI_COMM_WORLD);
        printf("[SLAVE]: Sent SCC size %d. \n", dim);//STATUS: SOURCE: %d TAG: %d ERROR: %d\n",dim,status_dim.MPI_SOURCE, status_dim.MPI_TAG, status_dim.MPI_ERROR);
        printf("[SLAVE]: Sending SCC: ");
        for(int i = 0;i<dim;i++){
            printf("a[%d]= %d, ",i,array[i]);
        }
        printf("\n");
        MPI_Send(array,dim,MPI_INT,0,501,MPI_COMM_WORLD);
        printf("[SLAVE]: Sent SCC. %d\n", dim);//TAG: %d ERROR: %d\n",status_data.MPI_SOURCE, status_data.MPI_TAG, status_data.MPI_ERROR);

        free(array);
        sleep(1);
    }
}