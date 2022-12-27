#ifndef MPI_LOGIC_H
#define MPI_LOGIC_H

#include <stdio.h>  
#include <string.h>
#include "graph.h"

#define MASTER 0
#define DIM_CHUNK 2000

typedef enum {
    MPI_TAG_DATA = 0, //0
    MPI_TAG_SIZE = 1 //1
} mpi_tag_t;

void master_schedule(graph_t*,int,int,scc_set_t*);
void master_work(int rank,int size,char* filename,char* outputfilename);
void slave_work(int rank);
void master_work2(int rank,int size,graph_t* graph,scc_set_t * SCCs,char* outputfilename, double time_init);

#endif