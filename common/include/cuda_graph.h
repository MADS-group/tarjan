#ifndef CUDA_GRAPH_H
#define CUDA_GRAPH_H

#include <graph.h>

struct cuda_graph_t {
    int n_vertex;
    int *adj_lists;
    int *adj_list_indexes;
};

typedef struct cuda_graph_t cuda_graph_t;

cuda_graph_t *cuda_graph_load_from_file(char *);
graph_t *cuda_graph_to_graph(cuda_graph_t *, int *);
void cuda_graph_free(cuda_graph_t *);


#endif