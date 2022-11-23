#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "graph.h"
#include "array.h"
#include "args.h"

int main(int argc, char* argv[]){
    graph_t * t;
    t=graph_random(5, 2, 0.5);
    graph_print_debug(t);
    graph_free(t);
}