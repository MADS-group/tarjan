#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include <mpi.h>

#define MASTER 0

typedef enum {
    MPI_TAG_DATA = 0, //0
    MPI_TAG_SIZE = 1 //1
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
    //I processi con rank diverso da 0 sono nodi slave

    if(rank == 0){
        printf("Sono il master %d\n",rank);
        master_work(rank,size);
        
    }

    if(rank != 0){
        //codice degli slave
        printf("Sono lo slave %d\n",rank);
        slave_work(rank);
    }

    MPI_Finalize();
    return 0;
}


void master_work(int rank,int size){
    //codice del master
    MPI_Status status;
    MPI_Request request;
    MPI_Status status_data,status_size;
    MPI_Request request_data,request_size;
    graph_t* graph;
    khint_t pos = 0;
    int v_graph;
    int n_slaves = size-1;
    char filename[] = "../data/prova.bin"; //TODO: Da prendere in input
    int verteces_per_slave;
    int remainder_of_the_division;
    array_int* array;
    
    array_int* scc;
    scc_set_t *SCCs = scc_set_init(); //Set di SCC noti
    int scc_id;
    int still_working = n_slaves; //rappresenta il numero di precessi slave che stanno lavorando.
    int msg_size;
    int scc_size;
    int done;
    int flag_size = 0;

    graph = graph_load_from_file(filename);
    if(graph == NULL){
        printf("Path non trovato");
    }else{
        graph_print_debug(graph);
    }
    v_graph = graph_get_num_vertex(graph);
    remainder_of_the_division = v_graph % n_slaves;
    verteces_per_slave = v_graph/n_slaves;
    
    
    for(int i = 0; i<n_slaves; i++){

        array = graph_serialize(graph, i==0 ? verteces_per_slave + remainder_of_the_division : verteces_per_slave , &pos); //Serializes at most n nodes of the graph starting from the specified bucket. Array contains [n_vertex id_1 adj_1 -1 id_2 adj2 -1] etc...
        //comunicazione dell'array agli slave
        msg_size = array_int_length(array);
        MPI_Isend(&msg_size,1,MPI_INT, i+1,MPI_TAG_SIZE,MPI_COMM_WORLD,&request);
        MPI_Wait(&request,&status);
        MPI_Isend(array_int_get_ptr(array),msg_size,MPI_INT,i+1,MPI_TAG_DATA,MPI_COMM_WORLD,&request);
        MPI_Wait(&request,&status);
    }
    
    while(still_working > 0){
        //I messaggi degli slave hanno tre TAG. Il primo tag è per i messaggi di servizio, il secondo è per i dati ed il terzo è per comunicare la size. 
        
        //Devo poter ricevere messaggi asincroni da qualunque nodo slave. I nodi slave mi inviano gli scc.
        //Ricevo un messaggio su un certo tag e devo decidere se gestire gli scc oppure 
        //se il processo slave ha terminato la sua esecuzione, devo decrementare still_working.
        //Da fare busy waiting su queste due receive
        
        flag_size = 0;
        MPI_Recv(&scc_size,1, MPI_INT,MPI_ANY_SOURCE,MPI_TAG_SIZE,MPI_COMM_WORLD,&status_size);
        printf("[MASTER] Receiving SCC of size %d from (status)SOURCE: %d, TAG:%d, ERROR:%d\n", scc_size,status_size.MPI_SOURCE, status_size.MPI_TAG,status_size.MPI_ERROR);
        
        if(scc_size == 0){
            printf("[MASTER] Recieved a 0-size message\n");
            still_working -=1;
            continue;
        }

        if(scc_size != 0){ //Ho trovato uno slave che ha finito. Ora devo lavorare sull'scc appena calcolato dallo slave 
            //Devo essere sicuro di prendere l'scc del nodo che ha fatto uscire dal while loop precendente con status_data.MPI_SOURCE.
            scc = array_int_init(scc_size);
            array_int_resize(scc, scc_size);
            MPI_Recv(array_int_get_ptr(scc),scc_size,MPI_INT,status_size.MPI_SOURCE,MPI_TAG_DATA,MPI_COMM_WORLD,&status_data);
            printf("[MASTER] Received a SCC with lenght %d with status: status_data.MPI_SOURCE: %d status_data.MPI_TAG: %d status_data.MPI_ERROR: %d \n", scc_size, status_data.MPI_SOURCE, status_data.MPI_TAG, status_data.MPI_ERROR);
            
            array_int_print(scc);
            
            //Devo aggiungere l'scc che ho trovato in una NUOVA struttura dati. Così da poter ricordare quali sono gli scc trovati
            scc_id = array_int_get_min(scc);
            graph_merge_vertices(graph,scc_id,scc);
            scc_set_add(SCCs,scc_id,scc);
            //scc_set_print_debug(SCCs);
            //graph_print_debug(graph);
            array_int_free(scc);
        }
        
        
        

        
        

    }
    //DEBUG
    scc_set_print_debug(SCCs);
    //free
    scc_set_free(SCCs);
}


MPI_Status slave_status_size,slave_status_data;
MPI_Request slave_request_size,slave_request_data;
void callback(array_int * scc){
    int scc_size = array_int_length(scc);

    printf("[SLAVE] request: %d, Sending size: %d\n",slave_request_size,scc_size);
    MPI_Isend(&scc_size,1,MPI_INT,MASTER,MPI_TAG_SIZE,MPI_COMM_WORLD,&slave_request_size);
    MPI_Wait(&slave_request_size,&slave_status_size); //Da capire se funziona sulla Isend
    printf("[SLAVE]: Sent SCC with size %d. STATUS: SOURCE: %d TAG: %d ERROR: %d\n",scc_size,slave_status_size.MPI_SOURCE, slave_status_size.MPI_TAG, slave_status_size.MPI_ERROR);
    printf("[SLAVE] request %d, Sending SCC: ",slave_request_data);
    array_int_print(scc);
    MPI_Isend(array_int_get_ptr(scc),array_int_length(scc),MPI_INT,MASTER,MPI_TAG_DATA,MPI_COMM_WORLD,&slave_request_data);
    MPI_Wait(&slave_request_data,&slave_status_data); //Da capire se funziona sulla Isend
    printf("[SLAVE]: Sent SCC. STATUS: SOURCE: %d TAG: %d ERROR: %d\n",slave_status_data.MPI_SOURCE, slave_status_data.MPI_TAG, slave_status_data.MPI_ERROR);

}

void slave_work(int rank){
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
    
    //Invio array_scc al master
    graph_tarjan_foreach(subgraph,&callback);

    scc_size=0;
    //Comunico al master che il nodo slave ha terminato
    MPI_Send(&scc_size,1,MPI_INT,master,MPI_TAG_SIZE,MPI_COMM_WORLD);
    printf("[SLAVE] %d ended\n", rank);
}