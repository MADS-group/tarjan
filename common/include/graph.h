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
void graph_tarjan_foreach(graph_t *G, void *f(array_int *));
array_int *graph_serialize(graph_t *, int n, khint_t * bucket); //Serializes at most n nodes of the graph starting from the specified bucket. Array contains [size_of_array-1 n_vertex id_1 adj_1 -1 id_2 adj2 -1] etc...
void graph_deserialize(graph_t *, array_int *buff); //Deserializes data from buffer buff
void graph_free(graph_t *);
void graph_merge(graph_t *to, graph_t *from, double p); //give a graph to and a graph from and merge both, return graph is in graph to
graph_t *graph_random(int max_n_node, int mean_edges, double variance_edges);
void graph_print_debug(graph_t *G);
graph_t * graph_copy(graph_t * from);



typedef struct scc_set_t scc_set_t;
scc_set_t *scc_set_init();
void scc_set_free(scc_set_t *set);
void scc_set_add(scc_set_t *scc_set, int scc_id, array_int *nodes);
void scc_set_print_debug(scc_set_t *S);
void scc_set_merge(scc_set_t *dest, scc_set_t *src);
bool scc_set_contains(scc_set_t *b, scc_set_t *a);
array_int *scc_set_serialize(scc_set_t *S);
void scc_set_deserialize(scc_set_t *S, array_int *buff);
void scc_set_save_to_file(scc_set_t *S, char *filename);
scc_set_t *scc_set_load_from_file(char *filename);

#endif