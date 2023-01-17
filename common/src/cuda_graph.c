/* 
 * Course: High Performance Computing 2022/2023 
 * 
 * Lecturer: Francesco Moscato  fmoscato@unisa.it 
 *
 * Group:
 * Langella	Antonio   0622702011   a.langella31@studenti.unisa.it
 * Marsico Michele   0622702012   m.marsico10@studenti.unisa.it
 * Paolino Salvatore   0622702016   s.paolino6@studenti.unisa.it 
 * Risi Davide   0622702013   d.risi2@studenti.unisa.it
 * 
 * Copyright (C) 2023 - All Rights Reserved 
 *
 * This file is part of Parallelize-Tarjan-MPI-CUDA. 
 *
 * Parallelize-Tarjan-MPI-CUDA is free software: you can redistribute it and/or modify 
 * it under the terms of the GNU General Public License as published by 
 * the Free Software Foundation, either version 3 of the License, or 
 * (at your option) any later version. 
 *
 * Parallelize-Tarjan-MPI-CUDA is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * GNU General Public License for more details. 
 *
 * You should have received a copy of the GNU General Public License 
 * along with Parallelize-Tarjan-MPI-CUDA.  If not, see <http://www.gnu.org/licenses/>. 
 * 
 */

#include <cuda_graph.h>
#include <bitarray.h>
#include <stdio.h>

cuda_graph_t *cuda_graph_load_from_file(char *filename){
    FILE *fp = fopen(filename, "r");
    if (fp == NULL){
       printf("Error opening file %s, aborting.\n", filename);   // Program exits if the file pointer returns NULL.
       return NULL;
    }
    int n;
    fread(&n, sizeof(int), 1, fp);
    array_int *array = array_int_init(n+1);
    array_int_set(array, 0, n); //Fill position 0
    array_int_resize(array, n+1); //Sets array length = n+1
    fread(array_int_get_ptr(array)+1, sizeof(int), n, fp); //Fill from position 1 to n
    //array_int_print(array);
    cuda_graph_t *G = (cuda_graph_t *) malloc(sizeof(cuda_graph_t));
    G->n_vertex = array_int_get(array, 1);
    G->adj_lists_len = array_int_get(array, 0) - 1 - (2 * G->n_vertex);
    G->adj_list_indexes = (int *) malloc(sizeof(int) * (G->n_vertex + 1));
    G->adj_lists = (int *) malloc(sizeof(int) * G->adj_lists_len);
    int inserted_vertices = 0;
    int inserted_edges = 0;
    int j = 1;
    for(int i = 0; i < G->n_vertex; i++){
        j+=2;
        //printf("j=%d\n",j);
        fflush(stdout);
        G->adj_list_indexes[inserted_vertices] = inserted_edges;
        while(array_int_get(array, j) != -1){
            G->adj_lists[inserted_edges] = array_int_get(array, j);
            inserted_edges++;
            j++;
            //printf("inserted_edges=%d\n",inserted_edges);
            fflush(stdout);
        }
        inserted_vertices++;
        //printf("inserted_vertices=%d\n",inserted_vertices);
        fflush(stdout);
    }
    G->adj_list_indexes[inserted_vertices] = inserted_edges;
    //cuda_graph_print_debug(G);
    fclose(fp);
    array_int_free(array);
    return G;
}

graph_t *cuda_graph_to_graph(cuda_graph_t *G, int *deleted_bitarray){
    graph_t *result_G = graph_init();
    int src_vertex, target_vertex;
    for(src_vertex = 0; src_vertex < G->n_vertex; src_vertex++){
        if(test_bit(deleted_bitarray, src_vertex)){
            continue;
        }
        graph_insert_vertex(result_G, src_vertex);
        for(int i = G->adj_list_indexes[src_vertex]; i<G->adj_list_indexes[src_vertex+1]; i++){
            target_vertex = G->adj_lists[i];
            if(test_bit(deleted_bitarray, target_vertex)){
                continue;
            }
            graph_insert_vertex(result_G, target_vertex);
            graph_insert_edge(result_G, src_vertex, target_vertex);
        }   
    }
    return result_G;
}

void cuda_graph_free(cuda_graph_t *G){
    free(G->adj_list_indexes);
    free(G->adj_lists);
    free(G);
}

void cuda_graph_print_debug(cuda_graph_t *G){
    printf("Adj list indexes:\n");
    for(int i = 0; i < G->n_vertex + 1; i++){
        printf("%d ", G->adj_list_indexes[i]);
    }
    printf("\nAdj lists:\n");
    for(int i = 0; i < G->adj_lists_len; i++){
        printf("%d ", G->adj_lists[i]);
    }
    printf("\n");
}

