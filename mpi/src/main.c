#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include <mpi.h>

typedef enum {
    MPI_TAG_DATA, //0
    MPI_TAG_SERVICE, //1
    MPI_TAG_SIZE //2
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
        printf("Sono il master %d\n",rank);
        master_work(rank,size);
        
    }

    if(rank != 0){
        //codice degli slave
        printf("Sono lo slave %d\n",rank);
        slave_work();
    }

    MPI_Finalize();
    return 0;
}


void master_work(int rank,int size){
    //codice del master
    MPI_Status status;
    MPI_Request request;
    MPI_Status status_data;
    MPI_Status status_service;
    MPI_Request request_data;
    MPI_Request request_service;
    graph_t* graph;
    khint_t pos = 0;
    int v_graph;
    int n_slaves = size-1;
    char filename[6] = "graph"; //TODO: Da prendere in input
    int verteces_per_slave;
    int remainder_of_the_division;
    array_int* array;
    
    array_int* scc;
    int still_working = n_slaves; //rappresenta il numero di precessi slave che stanno lavorando.
    int msg_size;
    int scc_size;
    int done;
    int flag_size,flag_service;

    graph = graph_load_from_file(filename);
    v_graph = graph_get_num_vertex(graph);
    remainder_of_the_division = v_graph % n_slaves;
    verteces_per_slave = v_graph/n_slaves;
    
    
    for(int i = 0; i<n_slaves; i++){

        array = graph_serialize(graph, i==0 ? verteces_per_slave + remainder_of_the_division:verteces_per_slave , &pos); //Serializes at most n nodes of the graph starting from the specified bucket. Array contains [n_vertex id_1 adj_1 -1 id_2 adj2 -1] etc...
        //comunicazione dell'array agli slave
        msg_size = array_int_length(array);
        MPI_Isend(&msg_size,1,MPI_INT, i+1,MPI_TAG_SIZE,MPI_COMM_WORLD,&request);
        MPI_Wait(&request,&status);
        MPI_Isend(array_int_get_ptr(array),msg_size,MPI_INT,i+1,MPI_TAG_DATA,MPI_COMM_WORLD,&request);
    }

    while(still_working > 0){
        //I messaggi degli slave hanno tre TAG. Il primo tag è per i messaggi di servizio, il secondo è per i dati ed il terzo è per comunicare la size. 
        
        //Devo poter ricevere messaggi asincroni da qualunque nodo slave. I nodi slave mi inviano gli scc.
        //Ricevo un messaggio su un certo tag e devo decidere se gestire gli scc oppure 
        //se il processo slave ha terminato la sua esecuzione, devo decrementare still_working.

        while(!flag_service && !flag_size){ //Se non ci sono stati nuovi messaggi e c'è almeno uno slave ancora in esecuzione
            //Da fare busy waiting su queste due receive
            MPI_Irecv(&scc_size,1, MPI_INT,MPI_ANY_SOURCE,MPI_TAG_SIZE,MPI_COMM_WORLD,&request_data);
            //Se il secondo test è andato a buon fine -> devo decrementare still_working
            MPI_Irecv(&done,1,MPI_INT,MPI_ANY_SOURCE,MPI_TAG_SERVICE,MPI_COMM_WORLD,&request_service);

            MPI_Test(&request_data, &flag_size, &status_data);
            MPI_Test(&request_service, &flag_service, &status_service);
        }

        if(flag_size){ //Ho trovato uno slave che ha finito. Ora devo lavorare sull'scc appena calcolato dallo slave 
            
            //Devo essere sicuro di prendere l'scc del nodo che ha fatto uscire dal while loop precendente con status_data.MPI_SOURCE.
            MPI_Irecv(scc,scc_size,MPI_INT,status_data.MPI_SOURCE,MPI_TAG_DATA,MPI_COMM_WORLD,&request_data);
            MPI_Wait(&request_data,&status_data);

            //Devo aggiungere l'scc che ho trovato in una NUOVA struttura dati. Così da poter ricordare quali sono gli scc trovati
            graph_merge_vertices(graph, 0 /*int dest ???  BOHHHHHHHH ????*/ ,scc);

        }
        
        if(flag_service){
            still_working -=1;
        }
        

        
        

    }
}

void slave_work(){
    graph_t* subgraph = graph_init();
    array_int* array_scc; 
    int msg_size;
    int scc_size;
    int master = 0;
    int done = 1;
    array_int *buff;
    MPI_Request slave_request;
    MPI_Status slave_status;
    //Aspetto di ricevere l'array da deserialize
    MPI_Irecv(&msg_size,1,MPI_INT,0,MPI_TAG_SIZE,MPI_COMM_WORLD,&slave_request);
    MPI_Wait(&slave_request,&slave_status);
    MPI_Irecv(buff,msg_size,MPI_INT,0, MPI_TAG_DATA,MPI_COMM_WORLD,&slave_request);
    MPI_Wait(&slave_request,&slave_status);
    //deserializzo il grafo inviato dal master
    graph_deserialize(subgraph, buff);
    //Compute scc for the given subgraph
    array_scc = graph_tarjan(subgraph);

    //Invio array_scc al master
    scc_size = array_int_length(array_scc);
    MPI_Isend(scc_size,1,MPI_INT,master,MPI_TAG_SIZE,MPI_COMM_WORLD,&slave_request);
    MPI_Wait(&slave_request,&slave_status); //Da capire se funziona sulla Isend
    
    MPI_Isend(array_scc,array_int_length(array_scc),MPI_INT,master,MPI_TAG_DATA,MPI_COMM_WORLD,&slave_request);
    MPI_Wait(&slave_request,&slave_status); //Da capire se funziona sulla Isend
    //Comunico al master che il nodo slave ha terminato
    MPI_Isend(&done,1,MPI_INT,master,MPI_TAG_SERVICE,MPI_COMM_WORLD,&slave_request);
}