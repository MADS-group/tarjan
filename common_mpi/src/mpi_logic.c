#include "mpi_logic.h"
#include "measurement.h"
#include <mpi.h>

double time_split_graph = 0.0,time_merge_graph = 0.0;

void callback(array_int * scc){
    int scc_size = array_int_length(scc);

    //printf("[SLAVE] request: %d, Sending size: %d\n",slave_request_size,scc_size);
    MPI_Send(&scc_size,1,MPI_INT,MASTER,MPI_TAG_SIZE,MPI_COMM_WORLD);
    //MPI_Wait(&slave_request_size,&slave_status_size); //Da capire se funziona sulla Isend
    //printf("[SLAVE]: Sent SCC with size %d. STATUS: SOURCE: %d TAG: %d ERROR: %d\n",scc_size,slave_status_size.MPI_SOURCE, slave_status_size.MPI_TAG, slave_status_size.MPI_ERROR);
    //printf("[Slave] Sending scc: \n");
    //array_int_print(scc);
    MPI_Send(array_int_get_ptr(scc),array_int_length(scc),MPI_INT,MASTER,MPI_TAG_DATA,MPI_COMM_WORLD);
    //MPI_Wait(&slave_request_data,&slave_status_data); //Da capire se funziona sulla Isend
    //printf("[SLAVE]: Sent SCC. STATUS: SOURCE: %d TAG: %d ERROR: %d\n",slave_status_data.MPI_SOURCE, slave_status_data.MPI_TAG, slave_status_data.MPI_ERROR);

}

void master_schedule(graph_t* graph,int N,int n_slaves,scc_set_t *SCCs){
    //codice del master
    MPI_Status status_send_size,status_send_data;
    double partial_time_split = 0.0, partial_time_merge = 0.0;
    MPI_Status status_data,status_size;
    MPI_Request request_data,request_size;
    
    khint_t pos = 0;

    array_int* serialized_graph_chunk;
    
    array_int* scc;
    
    int scc_id;
    int still_working = 0; //rappresenta il numero di precessi slave che stanno lavorando.
    int msg_size;
    int scc_size;
    int done,finished = 0; //Inizializzo finish a 0


    for(int i = 0; i<n_slaves; i++){
        STARTTIME(4);
        serialized_graph_chunk = graph_serialize(graph, N , &pos); //Serializes at most n nodes of the graph starting from the specified bucket. Array contains [n_vertex id_1 adj_1 -1 id_2 adj2 -1] etc...
        ENDTIME(4,partial_time_split);
        time_split_graph += partial_time_split;
        msg_size = array_int_length(serialized_graph_chunk);

        if(array_int_get(serialized_graph_chunk, 1) != 0){ //Se è 0 vuol dire che non ho più chunk da asseganare agli slave perché la struttura dati ha esaurito vertici
            still_working++;
        }else{
            finished = 1;
            break;
        }
        //comunicazione dell'array agli slave
        MPI_Send(&msg_size,1,MPI_INT, i+1,MPI_TAG_SIZE,MPI_COMM_WORLD);
        //printf("[MASTER] Sending array with size %d: ",msg_size);
        //array_int_print(serialized_graph_chunk);
        //printf("\n");
        MPI_Send(array_int_get_ptr(serialized_graph_chunk),msg_size,MPI_INT,i+1,MPI_TAG_DATA,MPI_COMM_WORLD);

    }
    
    while(!finished || still_working != 0){
        
        /*I messaggi degli slave hanno tre TAG. Il primo tag è per i messaggi di servizio, il secondo è per i dati ed il terzo è per comunicare la size. 
        
        //Devo poter ricevere messaggi asincroni da qualunque nodo slave. I nodi slave mi inviano gli scc.
        //Ricevo un messaggio su un certo tag e devo decidere se gestire gli scc oppure 
        //se il processo slave ha terminato la sua esecuzione, devo decrementare still_working.
        //Da fare busy waiting su queste due receive
        */
        //printf("finished: %d, still_working: %d\n",finished,still_working);
        //printf("[MASTER] Before Receiving");
        MPI_Recv(&scc_size,1, MPI_INT,MPI_ANY_SOURCE,MPI_TAG_SIZE,MPI_COMM_WORLD,&status_size);
        //printf("[MASTER] Receiving SCC of size %d from (status)SOURCE: %d, TAG:%d, ERROR:%d\n", scc_size,status_size.MPI_SOURCE, status_size.MPI_TAG,status_size.MPI_ERROR);
        
        if(scc_size == 0 && !finished){
            STARTTIME(5);
            serialized_graph_chunk = graph_serialize(graph, N , &pos); //Serializes at most n nodes of the graph starting from the specified bucket. Array contains [n_vertex id_1 adj_1 -1 id_2 adj2 -1] etc...
            ENDTIME(5,partial_time_split);
            time_split_graph += partial_time_split;
            msg_size = array_int_length(serialized_graph_chunk);
            //printf("[MASTER] if. scc_size: %d, finished: %d, n_serialized: %d\n",scc_size,finished,array_int_get(serialized_graph_chunk, 1));
            if(array_int_get(serialized_graph_chunk, 1) == 0){ //Se la msg_size è 0 vuol dire che non ho più chunk da asseganare agli slave perché la struttura dati ha esaurito vertici
                finished = 1;
                still_working--;
            }else{ 
                //Assegno allo slave che ha finito un altro chunck 
                //printf("[MASTER] Sending another chunck of size: %d\n",msg_size);
                MPI_Send(&msg_size,1,MPI_INT,status_size.MPI_SOURCE,MPI_TAG_SIZE,MPI_COMM_WORLD);        
                MPI_Send(array_int_get_ptr(serialized_graph_chunk),msg_size,MPI_INT,status_size.MPI_SOURCE,MPI_TAG_DATA,MPI_COMM_WORLD);
                //printf("[MASTER] Just Sent\n");
            }
        }else if(scc_size == 0){
            //printf("[MASTER] else if. scc_size: %d, finished: %d\n",scc_size,finished);
            //printf("[MASTER] decrementing still_working...\n");
            still_working--;
        }else{
            scc = array_int_init(scc_size);
            array_int_resize(scc, scc_size);
            MPI_Recv(array_int_get_ptr(scc),scc_size,MPI_INT,status_size.MPI_SOURCE,MPI_TAG_DATA,MPI_COMM_WORLD,&status_data);
            
            STARTTIME(6);

            //DEBUG
            //printf("[MASTER] Received a SCC with lenght %d with status: status_data.MPI_SOURCE: %d status_data.MPI_TAG: %d status_data.MPI_ERROR: %d \n", scc_size, status_data.MPI_SOURCE, status_data.MPI_TAG, status_data.MPI_ERROR);
            //array_int_print(scc);
            
            //Devo aggiungere l'scc che ho trovato in una NUOVA struttura dati. Così da poter ricordare quali sono gli scc trovati
            scc_id = array_int_get_min(scc);
            //printf("[MASTER] scc_id: %d\n",scc_id);
            //graph_print_debug(graph);
            //array_int_print(scc);
            graph_merge_vertices(graph,scc_id,scc);
            //printf("[MASTER] graph merged\n");
            scc_set_add(SCCs,scc_id,scc);
            //printf("[MASTER] scc_set_add \n");

            //DEBUG
            //scc_set_print_debug(SCCs);
            //graph_print_debug(graph);
            array_int_free(scc);
            ENDTIME(6,partial_time_merge);
            time_merge_graph += partial_time_merge;
        }
    }

    

}

void master_work(int rank,int size,char* filename,char* outputfilename){
    graph_t* graph;
    int v_graph,num_vertex;
    double time_init,time_mpi_tarjan;

    STARTTIME(1);
    scc_set_t *SCCs = scc_set_init(); //Set di SCC noti
    graph = graph_load_from_file(filename);
    
    if(graph == NULL){
        printf("[MASTER] Path not found\n");
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }
    ENDTIME(1,time_init);

    STARTTIME(2);
    //graph_print_debug(graph);
    int i;
    for(i = DIM_CHUNK; i<=graph_get_num_vertex(graph); i *= 2){
        master_schedule(graph,i,size-1,SCCs);
    }
    if(i>graph_get_num_vertex(graph)){ //Ensure last loop executes on the enterity of the graph.
        i=graph_get_num_vertex(graph);
        master_schedule(graph,i,size-1,SCCs);
    }
    ENDTIME(2,time_mpi_tarjan);
    num_vertex = graph_get_num_vertex(graph);
    printf("%d,%f,%f,%f,%f,",num_vertex,time_init,time_mpi_tarjan,time_split_graph,time_merge_graph);
    //DEBUG
    //scc_set_print_debug(SCCs);
    scc_set_save_to_file(SCCs,outputfilename);

    //Stop the slaves
    int abort_message = 0;
    //printf("[MASTER] Sending abort messages\n");
    for(i = 1; i < size; i++){
      MPI_Send(&abort_message,1,MPI_INT,i,MPI_TAG_SIZE,MPI_COMM_WORLD);
    }
    //free
    scc_set_free(SCCs);
    graph_free(graph);


}

void slave_work(int rank){
    graph_t* subgraph;
    array_int* array_scc; 
    int msg_size;
    int scc_size;
    int master = 0;
    int done = 1;
    array_int *buff;
    //int* array;
    MPI_Request slave_request;
    MPI_Status slave_status;


    while(1){
        //Aspetto di ricevere l'array da deserialize
        MPI_Recv(&msg_size,1,MPI_INT,0,MPI_TAG_SIZE,MPI_COMM_WORLD,&slave_status);
        //printf("[SLAVE] recieved msg_size=%d\n",msg_size);
        if(msg_size == 0){
          //printf("[SLAVE] Aborting\n");
          return;
        }
        //printf("\n[slave] msg_size: %d\n",msg_size);
        
        buff = array_int_init(msg_size);
        array_int_resize(buff, msg_size);
        subgraph = graph_init();
        MPI_Recv(array_int_get_ptr(buff),msg_size,MPI_INT,0, MPI_TAG_DATA,MPI_COMM_WORLD,&slave_status);

        //deserializzo il grafo inviato dal master
        graph_deserialize(subgraph, buff);
        array_int_free(buff);
        //Compute scc for the given subgraph
        
        //DEBUG
        //printf("[SLAVE] Printing subgraph...\n");
        //graph_print_debug(subgraph);

        //Invio array_scc al master
        graph_tarjan_foreach(subgraph,&callback);

        scc_size=0;
        //Comunico al master che il nodo slave ha terminato
        MPI_Send(&scc_size,1,MPI_INT,master,MPI_TAG_SIZE,MPI_COMM_WORLD);
        //printf("[SLAVE] rank: %d ended\n", rank);
        graph_free(subgraph);
    }
}


