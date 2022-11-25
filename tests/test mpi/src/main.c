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
    
    MPI_Status status;
    MPI_Request request;

    while(1){


        MPI_Irecv(&scc_size,1, MPI_INT,MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,);

        //if()
        if(flag_size){
            printf("[MASTER] I received a size: %d with status: status_data.MPI_SOURCE: %d status_data.MPI_TAG: %d status_data.MPI_ERROR: %d \n", scc_size, status_data.MPI_SOURCE, status_data.MPI_TAG, status_data.MPI_ERROR);
        }

        if(flag_size){ //Ho trovato uno slave che ha finito. Ora devo lavorare sull'scc appena calcolato dallo slave 
            //Devo essere sicuro di prendere l'scc del nodo che ha fatto uscire dal while loop precendente con status_data.MPI_SOURCE.
            scc = array_int_init(scc_size);
            printf("[MASTER] Receiving SCC of size %d \n", scc_size);
            array_int_resize(scc, scc_size);
            MPI_Irecv(array_int_get_ptr(scc),scc_size,MPI_INT,status_data.MPI_SOURCE,MPI_TAG_DATA,MPI_COMM_WORLD,&request_data);
            printf("[MASTER] Irevc called\n");
            MPI_Wait(&request_data,&status_data);
            printf("[MASTER] Received a SCC with status: status_data.MPI_SOURCE: %d status_data.MPI_TAG: %d status_data.MPI_ERROR: %d \n", scc_size, status_data.MPI_SOURCE, status_data.MPI_TAG, status_data.MPI_ERROR);
            printf("[MASTER] printing scc with length %d...\n",scc_size);
            array_int_print(scc);
            //Devo aggiungere l'scc che ho trovato in una NUOVA struttura dati. Così da poter ricordare quali sono gli scc trovati
            scc_id = array_int_get_min(scc);
            graph_merge_vertices(graph,scc_id,scc);
            scc_set_add(SCCs,scc_id,scc);
            scc_set_print_debug(SCCs);
            graph_print_debug(graph);
            array_int_free(scc);
        }
        
        if(flag_service){
            still_working -=1;
        }
        





    }


}


void slave_work(){



    while(1){

        graph_t* subgraph = graph_init();
        array_int* array_scc; 
        int msg_size;
        int scc_size;
        int master = 0;
        int done = 1;
        array_int *buff;
        //int* array;
        MPI_Request slave_request;
        MPI_Status slave_status;
        //Aspetto di ricevere l'array da deserialize
        MPI_Irecv(&msg_size,1,MPI_INT,0,MPI_TAG_SIZE,MPI_COMM_WORLD,&slave_request);
        MPI_Wait(&slave_request,&slave_status);
        
        printf("\n[slave] msg_size: %d\n",msg_size);
        
        buff = array_int_init(msg_size);
        array_int_resize(buff, msg_size);
        MPI_Irecv(array_int_get_ptr(buff),msg_size,MPI_INT,0, MPI_TAG_DATA,MPI_COMM_WORLD,&slave_request);
        MPI_Wait(&slave_request,&slave_status);
        printf("[Slave] array =\n");
        array_int_print(buff);
        //deserializzo il grafo inviato dal master
        graph_deserialize(subgraph, buff);
        array_int_free(buff);
        //Compute scc for the given subgraph
        
        //array_scc = graph_tarjan(subgraph);
        //printf("Slave: printing scc\n");
        //array_int_print(array_scc);
        //printf("\n");
        //buff = array_int_init(graph_get_num_vertex(subgraph));

        //Invio array_scc al master
        graph_tarjan_foreach(subgraph,&callback);

        //Comunico al master che il nodo slave ha terminato
        MPI_Isend(&done,1,MPI_INT,master,MPI_TAG_SERVICE,MPI_COMM_WORLD,&slave_request);






    }
}