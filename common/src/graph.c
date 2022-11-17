/*Implementazione di un grafo*/
#include <assert.h>
#include "graph.h"
#include "stack.h"
#include "khash.h"
#include "linkedlist.h"

KHASH_MAP_INIT_INT(m32, int) //m32 is a hash table with int keys and int values
KHASH_MAP_INIT_INT(mm32, khash_t(m32) *) //mm32 is a hash table with int keys and (m32 *) values

struct graph_t {
    int n_vertex;
    khash_t(mm32) *adj; //Hash table which maps a node with the forward adjacency list
    khash_t(mm32) *inverted_adj; //Hash table which maps a node with the backwards adjacency list
    
};


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

void graph_insert_edge(graph_t *G, int u, int v){
    khint_t k;
    bool is_present;
    int ret;

    k = kh_get(mm32, G->adj, u);
    is_present = (k != kh_end(G->adj));
    assert(is_present); //assert u exists
    khash_t(m32) *adj_list = kh_value(G->adj,k);
    k = kh_put(m32, adj_list, v, &ret);//ret == 0 if v is already present in the ht, ret>0 otherwise
    assert(ret != 0); //assert edge didn't exist
    kh_value(adj_list,k) = 42; //TODO: convert map to set

    k = kh_get(mm32, G->inverted_adj, v);
    is_present = (k != kh_end(G->inverted_adj));
    assert(is_present); //assert v exists
    khash_t(m32) *adj_list = kh_value(G->inverted_adj,k);
    k = kh_put(m32, adj_list, u, &ret);//ret == 0 if u is already present in the ht, ret>0 otherwise
    assert(ret != 0); //assert edge didn't exist
    kh_value(adj_list,k) = 42; //TODO: convert map to set
}


array_int *graph_tarjan(graph_t *G){
    int *disc = malloc(sizeof(int)*G->n_vertex);
    int *low = malloc(sizeof(int)*G->n_vertex);
    bool *stackMember = malloc(sizeof(bool)*G->n_vertex);
    linkedlist_int *stack = linkedlist_int_init();
    array_int *result = array_int_init(20);
    free(disc);
    free(low);
    free(stackMember);
}







/*


typedef enum graphi_type_t {
    GRAPHI_TYPE_DFS,
    GRAPHI_TYPE_CHILDREN
} graphi_type_t;

typedef struct graphi_t {
    graph_t *graph; //Pointer to the graph to iterate
    int i; //Counter of visited elements
    graphi_type_t type; //Type of the iterator

    //DFS Type iterator
    stack_t *stack; //Stack for DFS

    //CHILDREN Type iterator
    int *last; //Pointer to the last element returned
};
*/
/*Consider simplifying the design of graph.c. 
 - What is the real advantage of this complex generalization?
 - How many implementations of graph are we really going to mantain?
 - How many versions of tarjan are we going to use?
 - Are we using SCC algorithms other than tarjan?
 - What operations will be done on a graph?
*/