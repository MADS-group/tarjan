/*Implementazione di un grafo*/
#include "graph.h"
#include "stack.h"

struct graph_t {
    int n_vertex;
    array_ptr *adj; //Dynamic array of pointers to adjacency lists
    array_ptr *inverted_adj; //Inverted index

};















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