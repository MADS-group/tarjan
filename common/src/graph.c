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
    adj_list = kh_value(G->inverted_adj,k);
    k = kh_put(m32, adj_list, u, &ret);//ret == 0 if u is already present in the ht, ret>0 otherwise
    assert(ret != 0); //assert edge didn't exist
    kh_value(adj_list,k) = 42; //TODO: convert map to set
}

void graph_tarjan_helper(graph_t *G, int node, khash_t(m32) *disc, khash_t(m32) *low,
   linkedlist_int *stack, khash_t(m32) *stackMember,int *time, array_int *result){ 
    khint_t k, j;
    int adj_node, temp;
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
    kh_foreach(adjacency_list, adj_node, temp, {
        k = kh_get(m32,disc,adj_node);
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

array_int *graph_tarjan(graph_t *G){
    khash_t(m32) *disc = kh_init(m32);
    khash_t(m32) *low = kh_init(m32);
    khash_t(m32) *stackMember = kh_init(m32);
    linkedlist_int *stack = linkedlist_int_init();
    int time = 0;
    array_int *result = array_int_init((G->n_vertex)*2);

    int node,_;
    khash_t(m32) *temp;
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