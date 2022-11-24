#ifndef GRAPH_H
#define GRAPH_H

#include <stdbool.h>
#include "array.h"
#include "khash.h"

struct graph_t {
    int n_vertex;
    khash_t(mm32) *adj; //Hash table which maps a node with the forward adjacency list //TODO: convert to s32 * values!
    khash_t(mm32) *inverted_adj; //Hash table which maps a node with the backwards adjacency list
};

typedef struct graph_t graph_t;


graph_t *graph_init();
int graph_get_num_vertex(graph_t *);
void graph_insert_vertex(graph_t *, int v); //Insert a vertex v
void graph_insert_edge(graph_t *, int u, int v); //Insert a directed edge from vertex u to vertex v
void graph_delete_vertex(graph_t *, int v); //Delete a vertex v
void graph_delete_edge(graph_t *, int u, int v); //Delete the directed edge from vertex u to vertex v
void graph_merge_vertices(graph_t *, int dest, array_int *src); //Merge the vertices contained in 'src' in the vertex 'dest'.
graph_t *graph_load_from_file(char *);
void graph_save_to_file(graph_t *, char *);
array_int *graph_tarjan(graph_t *); //Returns an array containing the SCCs separated by -1
array_int *graph_serialize(graph_t *, int n, khint_t * bucket); //Serializes at most n nodes of the graph starting from the specified bucket. Array contains [size_of_array-1 n_vertex id_1 adj_1 -1 id_2 adj2 -1] etc...
void graph_deserialize(graph_t *, array_int *buff); //Deserializes data from buffer buff
void graph_free(graph_t *);
void graph_merge(graph_t *to, graph_t *from, double p); //give a graph to and a graph from and merge both, return graph is in graph to
graph_t *graph_random(int max_n_node, int mean_edges, double variance_edges);
void graph_print_debug(graph_t *G);

// #define graph_tarjan_foreach_helper(G, node, disc, low, stack, stackMember,time, result, scc, code){                                \
//     khint_t k, j;                                                                                                                   \
//     int adj_node, _; (void) _;     \                
//     ++(*time);\
//     k = kh_get(m32,disc,node);\
//     kh_value(disc,k) = *time;\
//     k = kh_get(m32,low,node);\
//     kh_value(low,k) = *time;\
//     linkedlist_int_push(stack, node);\
//     k = kh_get(m32,stackMember,node);\
//     kh_value(stackMember,k) = 1;\
//     k = kh_get(mm32,G->adj,node);\
//     khash_t(m32) *adjacency_list = kh_value(G->adj,k);\
//     kh_foreach(adjacency_list, adj_node, _, {\
//         k = kh_get(m32,disc,adj_node);\
//         if( kh_value(disc,k) == -1 ){\
//             graph_tarjan_helper(G, adj_node, disc, low, stack, stackMember, time, result, scc, code);\

//             // Check if the subtree rooted with 'v' has a
//             // connection to one of the ancestors of 'u'
//             // Case 1 (per above discussion on Disc and Low value)
//             k = kh_get(m32,low,node);
//             j = kh_get(m32,low,adj_node);
//             kh_value(low,k) = min(kh_value(low,k),kh_value(low,j));
//         }
//         // Update low value of 'u' only of 'v' is still in stack
//         // (i.e. it's a back edge, not cross edge).
//         // Case 2 (per above discussion on Disc and Low value)
//         else if (kh_value(stackMember,kh_get(m32,stackMember,adj_node)) == 1){
//             k = kh_get(m32,low,node);
//             j = kh_get(m32,disc,adj_node);
//             kh_value(low,k) = min(kh_value(low,k),kh_value(disc,j));
//         }
//     });
 
//     // head node found, pop the stack and print an SCC
//     int w = 0; // To store stack extracted vertices
//     k = kh_get(m32,low,node);
//     j = kh_get(m32,disc,node);
//     if (kh_value(low,k) == kh_value(disc,j)){
//         while (linkedlist_int_length(stack) > 0 && linkedlist_int_top(stack) != node){
//             w = (int) linkedlist_int_pop(stack);
//             k = kh_get(m32,stackMember,w);
//             kh_value(stackMember, k) = 0; //false
//             array_int_push(result, w);
//         }
//         w = (int) linkedlist_int_pop(stack);
//         k = kh_get(m32,stackMember,w);
//         kh_value(stackMember, k) = 0; //false
//         array_int_push(result, w);
//         array_int_push(result,-1); //SCC terminator
//     }
// // }

// void graph_tarjan_helper(graph_t *G, int node, khash_t(m32) *disc, khash_t(m32) *low,
//    linkedlist_int *stack, khash_t(m32) *stackMember,int *time, array_int *result){ 
    

#define graph_tarjan_foreach(G, scc, code){     \
    graph_tarjan(G);                            \
                                                \
    code                                        \
}

typedef struct scc_set_t scc_set_t;
scc_set_t *scc_set_init();
void scc_set_free(scc_set_t *set);
void scc_set_add(scc_set_t *scc_set, int scc_id, array_int *nodes);
void scc_set_print_debug(scc_set_t *S);

#endif