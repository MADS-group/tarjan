#ifndef GRAPH_H
#define GRAPH_H

#include <stdbool.h>
#include "array.h"
#include "khash.h"

typedef struct graph_t graph_t;


graph_t *graph_init();
int graph_get_num_vertex(graph_t *);
void graph_insert_vertex(graph_t *, int v); //Insert a vertex v
void graph_insert_edge(graph_t *, int u, int v); //Insert a directed edge from vertex u to vertex v
void graph_merge(graph_t *, array_int *nodes); //Merge the nodes in a supernode
graph_t *graph_load_from_file(char *);
void graph_save_to_file(graph_t *, char *);
array_int *graph_tarjan(graph_t *); //Returns an array containing the SCCs separated by -1
array_int *graph_serialize(graph_t *, int n, khint_t * bucket); //Serializes at most n nodes of the graph starting from the specified bucket. Array contains [n_vertex id_1 adj_1 -1 id_2 adj2 -1] etc...
void graph_deserialize(graph_t *, array_int *buff); //Deserializes data from buffer buff
void graph_free(graph_t *);

/*
// An iterator over nodes of a graph
typedef struct graphi_t graphi_t;

graphi_t *graph_get_child(graph_t *, int); //Returns an iterator over child of a given vertex
graphi_t *graph_DFS(graph_t *); //Returns an iterator over the entire graph using DFS search. Probably impossible to do 
bool graphi_has_next(graphi_t *);
int graphi_next(graphi_t *);
void graphi_free(graphi_t *);
*/
#endif