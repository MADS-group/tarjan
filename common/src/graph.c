/*  
 * Graph implementation with adjacency maps. adj is an hash table containing adjacency sets
 * Author: Antonio Langella 
*/
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "graph.h"
#include "stack.h"
#include "khash.h"
#include "linkedlist.h"
#include "random.h"

KHASH_MAP_INIT_INT(m32, int) //m32 type is a hash table with int keys and int values
KHASH_MAP_INIT_INT(mm32, khash_t(m32) *) //mm32 type is a hash table with int keys and (m32 *) values //TODO: convert to s32 * values!
KHASH_SET_INIT_INT(s32) //s32 type is a set of integers
KHASH_MAP_INIT_INT(ms32, khash_t(s32) *) //ms32 type is a hash table with int keys and (s32 *) values

struct graph_t {
    int n_vertex;
    khash_t(mm32) *adj; //Hash table which maps a node with the forward adjacency list
    khash_t(mm32) *inverted_adj; //Hash table which maps a node with the backwards adjacency list
};

int min(int a, int b){
    return a<b ? a:b;
}

graph_t *graph_init(){
    graph_t *G = malloc(sizeof(graph_t));
    G->n_vertex=0;
    G->adj = kh_init(mm32);
    G->inverted_adj = kh_init(mm32);
    return G;
}

void graph_free(graph_t *G){
    kh_destroy(mm32, G->adj);
    kh_destroy(mm32, G->inverted_adj);
    free(G);
}

int graph_get_num_vertex(graph_t *G){
    return G->n_vertex;
}

void graph_insert_vertex(graph_t *G, int v){
    int ret;
    khint_t k;

    k = kh_put(mm32,G->adj,v,&ret); //ret == 0 if v is already present in the ht, ret>0 otherwise
    if(ret == 0)//if v was already present in the ht
        return;
    
    (G->n_vertex)++;
    kh_value(G->adj, k) = kh_init(m32); //create a new adjacency map for the node v
    //repeat the same for inverted_adj
    k = kh_put(mm32,G->inverted_adj,v,&ret); //ret == 0 if v is already present in the ht, ret>0 otherwise
    kh_value(G->inverted_adj, k) = kh_init(m32); //create a new adjacency map for the node v
}
/*
 * Throws an error is edge already exists
*/
void graph_insert_edge(graph_t *G, int u, int v){
    khint_t k;
    bool is_present;
    int ret;

    k = kh_get(mm32, G->adj, u);
    is_present = (k != kh_end(G->adj));
    assert(is_present); //assert u exists
    khash_t(m32) *adj_list = kh_value(G->adj,k);
    k = kh_put(m32, adj_list, v, &ret);//ret == 0 if v is already present in the ht, ret>0 otherwise
    printf("ret: %d\n", ret);
    assert(ret != 0); //assert edge didn't exist
    kh_value(adj_list,k) = 42; //TODO: convert map to set

    k = kh_get(mm32, G->inverted_adj, v);
    is_present = (k != kh_end(G->inverted_adj));
    assert(is_present); //assert v exists
    adj_list = kh_value(G->inverted_adj,k);
    k = kh_put(m32, adj_list, u, &ret);//ret == 0 if u is already present in the ht, ret>0 otherwise
    assert(ret != 0); //assert edge didn't exist
    kh_value(adj_list,k) = 42; //TODO: convert map to set
}

/*
 * Deletes an edge from node u to node v if it exists. Otherwise does nothing.
 */
void graph_delete_edge(graph_t *G, int u, int v){
    khint_t k;
    bool is_present;
    khash_t(m32) *adj_list;
    k = kh_get(mm32, G->adj, u);
    is_present = (k != kh_end(G->adj));
    if(is_present){
        adj_list = kh_value(G->adj,k); //Take the adjacency list
        k = kh_get(m32, adj_list, v);
        kh_del(m32,adj_list,k); //delete the u->v edge from outcoming edges of u
    }
    //Do the same for inverted_adj
    k = kh_get(mm32, G->inverted_adj, v);
    is_present = (k != kh_end(G->inverted_adj));
    if(is_present){
        adj_list = kh_value(G->inverted_adj,k); //Take the adjacency list
        k = kh_get(m32, adj_list, u);
        kh_del(m32,adj_list,k); //delete the u->v edge from incoming edges of v
    }
}

/*
 * Deletes a vertex and every edge incident on the vertex. 
 */
void graph_delete_vertex(graph_t *G, int v){
    int u, _; (void) _; //_ is a needed unused variable variable. We do this to silence -Wunused-but-set-variable warning
    khint_t k;
    khash_t(m32) *adj_list, *inv_adj_list, *temp;
    k = kh_get(mm32,G->adj,v);
    adj_list = kh_value(G->adj,k); //adj_list contains all the outcoming edges of v (v->*)
    k = kh_get(mm32,G->inverted_adj,v);
    inv_adj_list = kh_value(G->inverted_adj,k); //inv_adj_list contains all the incoming edges of v (*->v)

    //For each v->u edge delete the incoming edge of u
    kh_foreach(adj_list, u, _, { 
        k = kh_get(mm32,G->inverted_adj,u);
        temp = kh_value(G->inverted_adj,k);
        k = kh_get(m32, temp,v);
        kh_del(m32, temp, k);
    });

    //For each u->v edge delete the outcoming edge of u
    kh_foreach(inv_adj_list, u, _, { 
        k = kh_get(mm32,G->adj,u);
        temp = kh_value(G->adj,k); //Temp contains all the outcoming edges of u
        k = kh_get(m32, temp,v);
        kh_del(m32, temp, k);
    });

    //Dealloc adj_list and inv_adj_list
    kh_destroy(m32, adj_list);
    kh_destroy(m32, inv_adj_list);

    //Delete v from G->adj and G->inverted_adj
    k = kh_get(mm32, G->adj, v);
    kh_del(mm32, G->adj, k);
    k = kh_get(mm32, G->inverted_adj, v);
    kh_del(mm32, G->inverted_adj, k);
}

/*
 * See documentation for graph_tarjan()
 */
void graph_tarjan_helper(graph_t *G, int node, khash_t(m32) *disc, khash_t(m32) *low,
   linkedlist_int *stack, khash_t(m32) *stackMember,int *time, array_int *result){ 
    khint_t k, j;
    int adj_node, _; (void) _; //_ is a needed unused variable variable. We do this to silence -Wunused-but-set-variable warning
    // Initialize discovery time and low value
    ++(*time);
    k = kh_get(m32,disc,node);
    kh_value(disc,k) = *time;
    k = kh_get(m32,low,node);
    kh_value(low,k) = *time;

    linkedlist_int_push(stack, node);
    k = kh_get(m32,stackMember,node);
    kh_value(stackMember,k) = 1; //true

    // Go through all vertices adjacent to this
    k = kh_get(mm32,G->adj,node);
    khash_t(m32) *adjacency_list = kh_value(G->adj,k);
    kh_foreach(adjacency_list, adj_node, _, {
        k = kh_get(m32,disc,adj_node);
        //TODO: the above operation should fail when tarjan is executed on subgraphs of a given graph. Add a check.
        //^This is the point where one could decide to ignore absent vertices or even store them to optimize the algorithm later.
        if( kh_value(disc,k) == -1 ){
            graph_tarjan_helper(G, adj_node, disc, low, stack, stackMember, time, result);

            // Check if the subtree rooted with 'v' has a
            // connection to one of the ancestors of 'u'
            // Case 1 (per above discussion on Disc and Low value)
            k = kh_get(m32,low,node);
            j = kh_get(m32,low,adj_node);
            kh_value(low,k) = min(kh_value(low,k),kh_value(low,j));
        }
        // Update low value of 'u' only of 'v' is still in stack
        // (i.e. it's a back edge, not cross edge).
        // Case 2 (per above discussion on Disc and Low value)
        else if (kh_value(stackMember,kh_get(m32,stackMember,adj_node)) == 1){
            k = kh_get(m32,low,node);
            j = kh_get(m32,disc,adj_node);
            kh_value(low,k) = min(kh_value(low,k),kh_value(disc,j));
        }
    });
 
    // head node found, pop the stack and print an SCC
    int w = 0; // To store stack extracted vertices
    k = kh_get(m32,low,node);
    j = kh_get(m32,disc,node);
    if (kh_value(low,k) == kh_value(disc,j)){
        while (linkedlist_int_length(stack) >0 && linkedlist_int_top(stack) != node){
            w = (int) linkedlist_int_pop(stack);
            k = kh_get(m32,stackMember,w);
            kh_value(stackMember, k) = 0; //false
            array_int_push(result, w);
        }
        w = (int) linkedlist_int_pop(stack);
        k = kh_get(m32,stackMember,w);
        kh_value(stackMember, k) = 0; //false
        array_int_push(result, w);
        array_int_push(result,-1); //SCC terminator
    }
}
/* 
 * Tarjan's algorithm implementation using recursion.
 * This is a modified version of the algorithm on the geeksforgeeks.com website.
 * The main differences are:
 * - disc, low and stackMember are now hash tables because we remove the hypotesis that vertex ids go from 0 to N-1:
 *   when working on a subgraph (as a slave process), there are no guarantees on the order nor continuity of the vertex ids.
 *   Using hash tables instead of arrays we save a lot of memory.
 * - we remove the hypotesis that every vertex in an adjacency map exists in the graph. This is also caused by executions
 *   on subgraphs of a given graph.
 */
array_int *graph_tarjan(graph_t *G){
    khash_t(m32) *disc = kh_init(m32);
    khash_t(m32) *low = kh_init(m32);
    khash_t(m32) *stackMember = kh_init(m32);
    linkedlist_int *stack = linkedlist_int_init();
    int time = 0;
    array_int *result = array_int_init((G->n_vertex)*2);

    int node,_; (void) _; //_ is a needed unused variable variable. We do this to silence -Wunused-but-set-variable warning
    khash_t(m32) *temp; (void) temp; //temp is a needed unused variable variable. We do this to silence -Wunused-but-set-variable warning
    khint_t k;
    kh_foreach(G->adj, node, temp, {
        k = kh_put(m32,disc,node,&_);
        kh_value(disc,k) = -1;
        k = kh_put(m32,low,node,&_);
        kh_value(low,k) = -1;
        k = kh_put(m32,stackMember,node,&_);
        kh_value(stackMember,k) = 0; //false
    });
    kh_foreach(G->adj, node, temp, {
        k = kh_get(m32,disc,node);
        if(kh_value(disc,k) == -1)
            graph_tarjan_helper(G,node, disc, low, stack, stackMember, &time, result);
    });

    kh_destroy(m32,disc);
    kh_destroy(m32,low);
    kh_destroy(m32,stackMember);
    return result;
}

array_int *graph_serialize(graph_t *G, int n, khint_t * bucket){
    array_int *result = array_int_init(G->n_vertex);
    int words = 0, node_a, node_b, _, serialized = 0; (void) _; //_ is a needed unused variable variable. We do this to silence -Wunused-but-set-variable warning
    khash_t(m32) *adj_list;
    array_int_push(result, 0); //Placeholder for number of total words to read after the first one
    array_int_push(result,0); //Placeholder for number vertexes serialized
    words++;
    khint_t i;
    for(i = *bucket; i != kh_end(G->adj) && serialized < n; ++i){
        if(!kh_exist(G->adj, i))
            continue;
        node_a = kh_key(G->adj,i);
        adj_list = kh_value(G->adj,i);
        array_int_push(result,node_a);
        words++;
        kh_foreach(adj_list, node_b, _, {
            array_int_push(result,node_b);
            words++;
        });
        array_int_push(result,-1); //end of adj list
        words++;
        serialized++;
    }
    *bucket = i;
    array_int_set(result, 0, words);
    array_int_set(result, 1, serialized);
    return result;
}

void graph_deserialize(graph_t *G, array_int *buff){
    //int words = array_int_get(buff,0);
    int n_vertex = array_int_get(buff,1);
    int i = 2, node_a, node_b;
    for(int j = 0; j<n_vertex;j++){
        node_a = array_int_get(buff,i);
        graph_insert_vertex(G,node_a);
        do{
            i++;
            node_b = array_int_get(buff,i);
            if(node_b != -1){
                graph_insert_vertex(G,node_b); //TODO: this adds nodes that are not owned by the slave to the graph. This needs further investigation as it may not be a problem at all.
                graph_insert_edge(G,node_a,node_b);
            }
        } while(node_b != -1);
        i++;
    }
}

void graph_save_to_file(graph_t *G, char *filename){
    khint_t pos = 0;
    array_int *array = graph_serialize(G, G->n_vertex, &pos);
    FILE *fp = fopen(filename, "w");
    fwrite(array_int_get_ptr(array), sizeof(int), array_int_get(array,0)+1, fp);
    fclose(fp);
    array_int_free(array);
}

graph_t *graph_load_from_file(char *filename){
    FILE *fp = fopen(filename, "r");
    int n;
    fread(&n, sizeof(int), 1, fp);
    array_int *array = array_int_init(n+1);
    array_int_set(array, 0, n); //Fill position 0
    fread(array_int_get_ptr(array)+1, sizeof(int), n, fp); //Fill from position 1
    graph_t *graph = graph_init();
    graph_deserialize(graph, array);
    fclose(fp);
    array_int_free(array);
    return graph;
}

//src è un array di interi, ovvero la lista degli id dei nodi che devono rientrare nel supernodo, dest è l'id del supernodo che si va a creare
void graph_merge_vertices(graph_t *G, int dest, array_int *src){
    int src_node, cpy, _; (void) _; //_ is a needed unused variable variable. We do this to silence -Wunused-but-set-variable warning
    khash_t(m32) *adj_list_src, *adj_list_dest, *inv_adj_list_src, *inv_adj_list_dest;
    khint_t k;
    
    //Get dest node adjacency list and inverted adjacency list
    k = kh_get(mm32, G->adj, dest);
    adj_list_dest = kh_value(G->adj, k);
    k = kh_get(mm32, G->inverted_adj, dest);
    inv_adj_list_dest = kh_value(G->inverted_adj, k);

    //Merge each src_node into dest node
    for(int i=0; i < array_int_length(src); i++){
        src_node = array_int_get(src, i);
        if(src_node == dest) //If src_node is equal to dest no merge is needed
            continue;
        //Get src_node adjacency list and inverted adjacency list
        k = kh_get(mm32, G->adj, src_node);
        adj_list_src = kh_value(G->adj, k);
        k = kh_get(mm32, G->inverted_adj, src_node);
        inv_adj_list_src = kh_value(G->inverted_adj, k);

        //Copy each node of src_node's adjacency list into dest's
        kh_foreach(adj_list_src, cpy, _, {
            if(cpy == src_node) //Copying auto-referencing edges produces errors. We don't do that
                continue;
            kh_put(m32, adj_list_dest, cpy, &_); //We copy the node of adj_list_src to adj_list_dest
        });
        //Copy each node of src_node's inverted adjacency list into dest's
        kh_foreach(inv_adj_list_src, cpy, _, {
            if(cpy == src_node) //Copying auto-referencing edges produces errors. We don't do that
                continue;
            kh_put(m32, inv_adj_list_dest, cpy, &_); //We copy the node of inv_adj_list_src to inv_adj_list_dest
        });
        //Now it is safe to delete the src vertex
        graph_delete_vertex(G, src_node);
    }
}

/*
    khash_t(mm32) #mm32 è una ht key: int e val: *khash_t(m32)
    khash_t(m32) #m32 è una ht key: int e val: int

    # Prendere un elemento 
    khint_t k;
    k = kh_get(m32, ht, key); #1: tipo della ht 2:riferimento alla ht 3: key
    # Se key è assente dalla ht: k == kh_end(ht)
    value = kh_value(ht, k); #1: riferimento alla ht 2: variabile di appoggio
    
    #Modificare un elemento
    khint_t k;
    k = kh_get(m32, ht, key); #1: tipo della ht 2:riferimento alla ht 3: key
    # Se key è assente dalla ht: k == kh_end(ht)
    kh_value(ht, k) = value; #1: riferimento alla ht 2: variabile di appoggio

    #Inserire un elemento (se non esiste) e modificare value
    khint_t k;
    k = kh_put(m32,ht,key,&ret); #1: tipo della ht 2: riferimento alla ht 3: key che vuoi inserire 4: riferimento a una variabile intera
    # k è la posizione dove si trova la chave appena inserita se è stata inserita (senza valore) OPPURE la posizione dove già era presente la chiave se già c'era.
    # ret == 0 se l'elemento era già nella ht e ret>0 se invece non era presente (ed è stato inserito adesso)
    kh_value(ht, k) = value #1: riferimento alla ht 2: variabile di appoggio

    #Vedere se chiave è presente nella ht
    k = kh_get(m32, ht, key);
    kh_exist(ht, k); #restituisce true se esiste o false altrimenti

    #Per scorrere tutti gli elementi nella ht
    kh_foreach(ht, key, value, { #1: hash table, 2: variablie dove viene messa la key, 3: variabile dove viene messo il value, 4: blocco di codice eseguito per ogni coppia (key,value)
        #...
    });

    #Hash annidate - voglio ciclare tutti i nodi figli di a (a->*)
    k = kh_get(mm32, G->adj, a);
    adj_list = kh_value(G->adj, k);
    foreach(adj_list, b, _, {
        #fai qualcosa
    });

*/

/*! @function
  @abstract      merge 2 graph and the merged graph is in graph_t * to
  @param  to     graph with vertex index from 0 to graph_get_num_vertex(to)
  @param  from   graph with vertex index from 0 to graph_get_num_vertex(from)
  @param p       probability of create an edge between a node of graph from and a node of graph to  and viceversa
 */
void graph_merge(graph_t *to, graph_t *from, double p){ //give a graph to and a graph from and merge both, return graph is in graph to
    //pene turgido
    int i=0;
    int vertex=0;
    int initial_number_of_vertex_graph_to=graph_get_num_vertex(to);
    int initial_number_of_vertex_graph_from=graph_get_num_vertex(from);
    int key=0;
    int value=0;
    int opposite=0;
    int to_vertex=0;
    int new_dimension=0;
    khint_t k;
    khash_t(m32) *adj_list;

    srand ( time(NULL) );

    for(i=0; i<initial_number_of_vertex_graph_from; i++){
        k = kh_get(mm32, from->adj, i);
        adj_list = kh_value(from->adj, k);     //trovo adiacent list associata al vertice i

        graph_insert_vertex(to, i+initial_number_of_vertex_graph_to);

        kh_foreach(adj_list, key, value, {
            graph_insert_vertex(to, key+initial_number_of_vertex_graph_to);
            graph_insert_edge(to, i+initial_number_of_vertex_graph_to, key+initial_number_of_vertex_graph_to);
        });

        if(rand_bernoulli(p)){
            opposite= rand() % initial_number_of_vertex_graph_to;
            graph_insert_edge(to, i+initial_number_of_vertex_graph_to, opposite);
            
        }
    }

    new_dimension=graph_get_num_vertex(to);

    for(i=initial_number_of_vertex_graph_to; i<new_dimension; i++){
        if(rand_bernoulli(p)){
            to_vertex=rand() % initial_number_of_vertex_graph_to;
            graph_insert_edge(to, to_vertex, i);
        }
    }
    
}
/*! @function
  @abstract      create a new graph with max_n_nodes vertices and number of edges for every vertex form a binomial distribution with mean and variance
  @param  max_n_node        number of vertices for the new graph
  @param  mean_edges        mean for binomial distribution used for number of edges for every vertices
  @param variance_edges     variance for binomial distribution used for number of edges for every vertices
  @return                   a graph with max_n_node vertices and number of edges for every vertices form binomial distribution(mean,variance)
 */

graph_t *graph_random(int max_n_node, int mean_edges, double variance_edges){ 
    int maxNumberOfEdges;
    graph_t *graph;
    int i=0;
    int j=0;
    int opposite=0;
    khint_t k;
    khash_t(m32) *adj_list;
    int ret=0;

    graph= graph_init();

    srand ( time(NULL) );

    for(i=0; i<max_n_node; i++){
        graph_insert_vertex(graph, i);
    }

    graph_print_debug(graph);

    for(i=0; i<max_n_node; i++){
        maxNumberOfEdges= rand_binomial_2(mean_edges,variance_edges);
        for(j=0; j<= maxNumberOfEdges; j++){
            opposite= rand() % max_n_node ;
            printf("numero di archi %d e arco opposite %d\n", maxNumberOfEdges,opposite);
            k = kh_get(mm32, graph->adj, i);
            adj_list = kh_value(graph->adj, k);     //trovo adiacent list associata al vertice j

            k=kh_put(m32,adj_list,opposite,&ret);    //cerco valore associato alla chiave oppiste (arco associato al nodo opposite)
            printf("ci sono\n");
            if(ret != 0){          //se edges non è presente lo aggiungo
            // k == kh_end(adj_list)
                printf("ret: %d\n", ret);
                printf("inserisco arco %d a %d\n",i , opposite);
                graph_insert_edge(graph, i, opposite);
            }else{                              //se edges è già presente ne aggiungerò un altro
                j--;
            }
        }
        /*    
            opposite= rand() % max_n_node ;
            printf("numero di archi %d e arco opposite %d\n", maxNumberOfEdges,opposite);
            k = kh_get(mm32, graph->adj, i);
            adj_list = kh_value(graph->adj, k);     //trovo adiacent list associata al vertice j

            k=kh_get(m32,adj_list,opposite);    //cerco valore associato alla chiave oppiste (arco associato al nodo opposite)
            printf("ci sono\n");
            if(! kh_exist(adj_list,k)){          //se edges non è presente lo aggiungo
            // k == kh_end(adj_list)
                printf("inserisco arco %d a %d\n",i , opposite);
                graph_insert_edge(graph, i, opposite);
            }else{                              //se edges è già presente ne aggiungerò un altro
                j--;
            }
        }
        */
    }

    return graph;

}

void graph_print_debug(graph_t *G){
    int key;
    khash_t(m32) *value;
    printf("Number of vertices: %d\n", G->n_vertex);
    printf("Adjacency list:\n");
    kh_foreach(G->adj, key, value, {
        printf("%d -->", key);
        for(khint_t i = 0; i != kh_end(value); ++i){
            if(!kh_exist(value, i))
                continue;
            printf(" %d", kh_key(value, i));
        }
        printf("\n");
    });

    printf("Inverted adjacency list:\n");
    kh_foreach(G->inverted_adj, key, value, {
        printf("%d -->", key);
        for(khint_t i = 0; i != kh_end(value); ++i){
            if(!kh_exist(value, i))
                continue;
            printf(" %d", kh_key(value, i));
        }
        printf("\n");
    });
}

struct scc_set_t {
    //??? Random thougths: what happens if i have an SCC 3 -> 4,5,6 and I find a new one 2 -> 3,11,12,13 ???
    khash_t(ms32) *scc_map; //The key is the lowest node in the SCC, the value is the set of nodes in the SCC (key included)
    khash_t(m32) *nodes_to_scc_map; //The key is a node of the graph, the value is the SCC where it belongs
};

/*! @function
  @abstract     Initialize a new scc_set
  @return       The scc_set 
 */
scc_set_t *scc_set_init(){
    scc_set_t *S = malloc(sizeof(scc_set_t));
    S->scc_map = kh_init(ms32);
    S->nodes_to_scc_map = kh_init(m32);
    return S;
}

/*! @function
  @abstract     Destroy an scc_set
  @param    S   The scc_set to be destroyed.
 */
void scc_set_free(scc_set_t *S){
    kh_destroy(ms32, S->scc_map);
    kh_destroy(m32, S->nodes_to_scc_map);
    free(S);
}

/*! @function
  @abstract     Add a new SCC to the set handling merges if needed.
  @param  S the reference to the scc_set.
  @param  scc_id the id of the SCC to be added. By convention, it is the lowest among the ids of the nodes in the SCC.
  @param  nodes the nodes of the SCC. 
 */
void scc_set_add(scc_set_t *S, int scc_id, array_int *nodes){
    int target_scc_id = scc_id, node, _, node_scc, ret, source_scc_id; (void)_;
    khash_t(s32) *scc_to_merge; //Set of all scc_ids that need to be merged
    khash_t(s32) *target_scc, *source_scc;
    khint_t k;

    scc_to_merge = kh_init(s32);
    kh_put(s32, scc_to_merge, scc_id, &_); //scc_id should be merged with other SCCs
    //Find target SCC and SCCs that need to be merged
    for(int i = 0; i < array_int_length(nodes); i++){
        node = array_int_get(nodes, i);
        k = kh_get(m32, S->nodes_to_scc_map, node); 
        if (!kh_exist(S->nodes_to_scc_map, k)) //If node is not present in an SCC, target_scc_id stays the same
            continue;
        //Otherwise we add the node scc to the set of SCCs to be merged...
        node_scc = kh_value(S->nodes_to_scc_map, k); //The SCC to which node belongs to
        kh_put(s32, scc_to_merge, node_scc, &_);
        //...and we check if target_scc_id should change (it is always the lowest among SCC IDs).
        target_scc_id = min(target_scc_id, node_scc);
    }

    //Get target_scc with id target_scc_id 
    k = kh_put(ms32, S->scc_map, target_scc_id, &ret);
    if(ret!=0){ //If target_scc_id doesn't exist, create it
        kh_value(S->scc_map, k) = kh_init(s32);
    }
    target_scc = kh_value(S->scc_map, k);

    //Move every node in nodes to target_scc
    for(int i = 0; i < array_int_length(nodes); i++){
        node = array_int_get(nodes, i);
        //Put the node in the target SCC
        kh_put(s32, target_scc, node, &_);
        //Update nodes_to_scc_map
        k = kh_put(m32, S->nodes_to_scc_map, node, &_);
        kh_value(S->nodes_to_scc_map, k) = target_scc_id;
    }

    //Move every node in scc_map[scc_to_merge[*]] to target_scc
    int cpy;
    khint_t source_scc_bucket;
    for(khint_t i = 0; i != kh_end(scc_to_merge); ++i){
        if(!kh_exist(scc_to_merge, i))
            continue;
        source_scc_id = kh_key(scc_to_merge, i);
        if(source_scc_id == target_scc_id) //If the source scc is the same as target do nothing
            continue;
        source_scc_bucket = kh_get(ms32, S->scc_map, source_scc_id);
        source_scc = kh_value(S->scc_map, source_scc_bucket); //source_scc should always exist because we populated scc_to_merge set previously
        for(khint_t j = 0; j != kh_end(source_scc); ++j){ //Put every element of source_scc in target SCC and update nodes_to_scc_map
            if(!kh_exist(source_scc, j))
                continue;
            cpy = kh_key(source_scc, j);
            //Put the node in the target SCC
            kh_put(s32, target_scc, cpy, &_); 
            //Update nodes_to_scc_map
            k = kh_put(m32, S->nodes_to_scc_map, cpy, &_);
            kh_value(S->nodes_to_scc_map, k) = target_scc_id;
        }
        //Now we can delete the SCC with id source_scc_id which is in bucket source_scc_bucket
        kh_destroy(s32, source_scc);
        kh_del(ms32, S->scc_map, source_scc_bucket);
    }
    kh_destroy(s32, scc_to_merge);
}

/*! @function
  @abstract     Debug print an scc_set
  @param  S the reference to the scc_set.
  @param  scc_id the id of the SCC to be added. By convention, it is the lowest among the ids of the nodes in the SCC.
  @param  nodes the nodes of the SCC. 
 */
void scc_set_print_debug(scc_set_t *S){
    int key;
    khash_t(s32) *value;
    kh_foreach(S->scc_map, key, value, {
        printf("%d -->", key);
        for(khint_t i = 0; i != kh_end(value); ++i){
            if(!kh_exist(value, i))
                continue;
            printf(" %d", kh_key(value, i));
        }
        printf("\n");
    });
    
}

/*Consider simplifying the design of graph.c. 
 - What is the real advantage of this complex generalization?
 - How many implementations of graph are we really going to mantain?
 - How many versions of tarjan are we going to use?
 - Are we using SCC algorithms other than tarjan?
 - What operations will be done on a graph?
*/