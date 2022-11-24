#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "graph.h"
#include "array.h"
#include "args.h"

int main(int argc, char* argv[]){
    graph_t * t, *c;
    t=graph_random(10, 2, 1);
    printf("\n\nseed generato:\n");
    graph_print_debug(t);
    graph_save_to_file(t, "./../../data/seed.bin");
    c=graph_load_from_file("./../../data/seed.bin");
    printf("\n\n seed letto c:\n");
    graph_print_debug(c);
}
///leggo seed da file e genero 