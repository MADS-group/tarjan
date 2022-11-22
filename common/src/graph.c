/*  
 * Graph implementation with adjacency maps. adj is an hash table containing adjacency sets
 * Author: Antonio Langella 
*/
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "graph.h"
#include "stack.h"
#include "khash.h"
#include "linkedlist.h"

KHASH_MAP_INIT_INT(m32, int) //m32 type is a hash table with int keys and int values
KHASH_MAP_INIT_INT(mm32, khash_t(m32) *) //mm32 type is a hash table with int keys and (m32 *) values //TODO: convert to s32 * values!
KHASH_SET_INIT_INT(s32) //s32 type is a set of integers

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
/*
 * Throws an error is edge already exists
*/
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

/*
 * Deletes an edge from node u to node v if it exists. Otherwise does nothing.
 */
void graph_delete_edge(graph_t *G, int u, int v){
    khint_t k;
    bool is_present;
    khash_t(m32) *adj_list;
    k = kh_get(mm32, G->adj, u);
    is_present = (k != kh_end(G->adj));
    if(is_present){
        adj_list = kh_value(G->adj,k); //Take the adjacency list
        k = kh_get(m32, adj_list, v);
        kh_del(m32,adj_list,k); //delete the u->v edge from outcoming edges of u
    }
    //Do the same for inverted_adj
    k = kh_get(mm32, G->inverted_adj, v);
    is_present = (k != kh_end(G->inverted_adj));
    if(is_present){
        adj_list = kh_value(G->inverted_adj,k); //Take the adjacency list
        k = kh_get(m32, adj_list, u);
        kh_del(m32,adj_list,k); //delete the u->v edge from incoming edges of v
    }
}

/*
 * Deletes a vertex and every edge incident on the vertex. 
 */
void graph_delete_vertex(graph_t *G, int v){
    int u, _; (void) _; //_ is a needed unused variable variable. We do this to silence -Wunused-but-set-variable warning
    khint_t k;
    khash_t(m32) *adj_list, *inv_adj_list, *temp;
    k = kh_get(mm32,G->adj,v);
    adj_list = kh_value(G->adj,k); //adj_list contains all the outcoming edges of v (v->*)
    k = kh_get(mm32,G->inverted_adj,v);
    inv_adj_list = kh_value(G->inverted_adj,k); //inv_adj_list contains all the incoming edges of v (*->v)

    //For each v->u edge delete the incoming edge of u
    kh_foreach(adj_list, u, _, { 
        k = kh_get(mm32,G->inverted_adj,u);
        temp = kh_value(G->inverted_adj,k);
        k = kh_get(m32, temp,v);
        kh_del(m32, temp, k);
    });

    //For each u->v edge delete the outcoming edge of u
    kh_foreach(inv_adj_list, u, _, { 
        k = kh_get(mm32,G->adj,u);
        temp = kh_value(G->adj,k); //Temp contains all the outcoming edges of u
        k = kh_get(m32, temp,v);
        kh_del(m32, temp, k);
    });

    //Dealloc adj_list and inv_adj_list
    kh_destroy(m32, adj_list);
    kh_destroy(m32, inv_adj_list);

    //Delete v from G->adj and G->inverted_adj
    k = kh_get(mm32, G->adj, v);
    kh_del(mm32, G->adj, k);
    k = kh_get(mm32, G->inverted_adj, v);
    kh_del(mm32, G->inverted_adj, k);
}

/*
 * See documentation for graph_tarjan()
 */
void graph_tarjan_helper(graph_t *G, int node, khash_t(m32) *disc, khash_t(m32) *low,
   linkedlist_int *stack, khash_t(m32) *stackMember,int *time, array_int *result){ 
    khint_t k, j;
    int adj_node, _; (void) _; //_ is a needed unused variable variable. We do this to silence -Wunused-but-set-variable warning
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
    kh_foreach(adjacency_list, adj_node, _, {
        k = kh_get(m32,disc,adj_node);
        //TODO: the above operation should fail when tarjan is executed on subgraphs of a given graph. Add a check.
        //^This is the point where one could decide to ignore absent vertices or even store them to optimize the algorithm later.
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
/* 
 * Tarjan's algorithm implementation using recursion.
 * This is a modified version of the algorithm on the geeksforgeeks.com website.
 * The main differences are:
 * - disc, low and stackMember are now hash tables because we remove the hypotesis that vertex ids go from 0 to N-1:
 *   when working on a subgraph (as a slave process), there are no guarantees on the order nor continuity of the vertex ids.
 *   Using hash tables instead of arrays we save a lot of memory.
 * - we remove the hypotesis that every vertex in an adjacency map exists in the graph. This is also caused by executions
 *   on subgraphs of a given graph.
 */
array_int *graph_tarjan(graph_t *G){
    khash_t(m32) *disc = kh_init(m32);
    khash_t(m32) *low = kh_init(m32);
    khash_t(m32) *stackMember = kh_init(m32);
    linkedlist_int *stack = linkedlist_int_init();
    int time = 0;
    array_int *result = array_int_init((G->n_vertex)*2);

    int node,_; (void) _; //_ is a needed unused variable variable. We do this to silence -Wunused-but-set-variable warning
    khash_t(m32) *temp; (void) temp; //temp is a needed unused variable variable. We do this to silence -Wunused-but-set-variable warning
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

array_int *graph_serialize(graph_t *G, int n, khint_t * bucket){
    array_int *result = array_int_init(G->n_vertex);
    int words = 0, node_a, node_b, _, serialized = 0; (void) _; //_ is a needed unused variable variable. We do this to silence -Wunused-but-set-variable warning
    khash_t(m32) *adj_list;
    array_int_push(result, 0); //Placeholder for number of total words to read after the first one
    array_int_push(result,0); //Placeholder for number vertexes serialized
    words++;
    khint_t i;
    for(i = *bucket; i != kh_end(G->adj) && serialized < n; ++i){
        if(!kh_exist(G->adj, i))
            continue;
        node_a = kh_key(G->adj,i);
        adj_list = kh_value(G->adj,i);
        array_int_push(result,node_a);
        words++;
        kh_foreach(adj_list, node_b, _, {
            array_int_push(result,node_b);
            words++;
        });
        array_int_push(result,-1); //end of adj list
        words++;
        serialized++;
    }
    *bucket = i;
    array_int_set(result, 0, words);
    array_int_set(result, 1, serialized);
    return result;
}

void graph_deserialize(graph_t *G, array_int *buff){
    //int words = array_int_get(buff,0);
    int n_vertex = array_int_get(buff,1);
    int i = 2, node_a, node_b;
    for(int j = 0; j<n_vertex;j++){
        node_a = array_int_get(buff,i);
        graph_insert_vertex(G,node_a);
        do{
            i++;
            node_b = array_int_get(buff,i);
            if(node_b != -1){
                graph_insert_vertex(G,node_b); //TODO: this adds nodes that are not owned by the slave to the graph. This needs further investigation as it may not be a problem at all.
                graph_insert_edge(G,node_a,node_b);
            }
        } while(node_b != -1);
        i++;
    }
}

void graph_save_to_file(graph_t *G, char *filename){
    khint_t pos = 0;
    array_int *array = graph_serialize(G, G->n_vertex, &pos);
    FILE *fp = fopen(filename, "w");
    fwrite(array_int_get_ptr(array), sizeof(int), array_int_get(array,0)+1, fp);
    fclose(fp);
    array_int_free(array);
}

graph_t *graph_load_from_file(char *filename){
    FILE *fp = fopen(filename, "r");
    int n;
    fread(&n, sizeof(int), 1, fp);
    array_int *array = array_int_init(n+1);
    array_int_set(array, 0, n); //Fill position 0
    fread(array_int_get_ptr(array)+1, sizeof(int), n, fp); //Fill from position 1
    graph_t *graph = graph_init();
    graph_deserialize(graph, array);
    fclose(fp);
    array_int_free(array);
    return graph;
}

void graph_merge_vertices(graph_t *G, int dest, array_int *src){
    int src_node, cpy, _; (void) _; //_ is a needed unused variable variable. We do this to silence -Wunused-but-set-variable warning
    khash_t(m32) *adj_list_src, *adj_list_dest, *inv_adj_list_src, *inv_adj_list_dest;
    khint_t k;
    
    //Get dest node adjacency list and inverted adjacency list
    k = kh_get(mm32, G->adj, dest);
    adj_list_dest = kh_value(G->adj, k);
    k = kh_get(mm32, G->inverted_adj, dest);
    inv_adj_list_dest = kh_value(G->inverted_adj, k);

    //Merge each src_node into dest node
    for(int i=0; i < array_int_length(src); i++){
        src_node = array_int_get(src, i);
        if(src_node == dest) //If src_node is equal to dest no merge is needed
            continue;
        //Get src_node adjacency list and inverted adjacency list
        k = kh_get(mm32, G->adj, src_node);
        adj_list_src = kh_value(G->adj, k);
        k = kh_get(mm32, G->inverted_adj, src_node);
        inv_adj_list_src = kh_value(G->inverted_adj, k);

        //Copy each node of src_node's adjacency list into dest's
        kh_foreach(adj_list_src, cpy, _, {
            if(cpy == src_node) //Copying auto-referencing edges produces errors. We don't do that
                continue;
            kh_put(m32, adj_list_dest, cpy, &_); //We copy the node of adj_list_src to adj_list_dest
        });
        //Copy each node of src_node's inverted adjacency list into dest's
        kh_foreach(inv_adj_list_src, cpy, _, {
            if(cpy == src_node) //Copying auto-referencing edges produces errors. We don't do that
                continue;
            kh_put(m32, inv_adj_list_dest, cpy, &_); //We copy the node of inv_adj_list_src to inv_adj_list_dest
        });
        //Now it is safe to delete the src vertex
        graph_delete_vertex(G, src_node);
    }
}



void graph_merge(graph_t *to, graph_t *from){ //give a graph to and a graph from and merge both, return graph is in graph to
}
graph_t *graph_random(int max_n_node, int max_edges){ //give max number of node, max number of edger for node and create a graph
    
}

/*Consider simplifying the design of graph.c. 
 - What is the real advantage of this complex generalization?
 - How many implementations of graph are we really going to mantain?
 - How many versions of tarjan are we going to use?
 - Are we using SCC algorithms other than tarjan?
 - What operations will be done on a graph?
*/