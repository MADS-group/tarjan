#ifndef GRAPH_H
#define GRAPH_H

#include <stdbool.h>
#include "array.h"


typedef struct graph_t graph_t;

graph_t *graph_init(int n);
graph_t *graph_read_from_file();
void graph_save_to_file(graph_t *);
array *graph_tarjan(graph_t *); //Returns an array containing the 
void graph_free(graph_t *);

// An iterator over nodes of a graph
typedef struct graphi_t graphi_t;

graphi_t *graph_get_child(graph_t *, int); //Returns an iterator over child of a given vertex
graphi_t *graph_DFS(graph_t *); //Returns an iterator over the entire graph using DFS search. Probably impossible to do 
bool graphi_has_next(graphi_t *);
int graphi_next(graphi_t *);
void graphi_free(graphi_t *);

#endif