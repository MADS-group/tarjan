#include <stdlib.h>
#include <stdio.h>
#include "graph.h"

int main(int argc, char* argv[]){
    FILE* file;
    file = fopen("test.txt", "r");

    if (NULL == file) {
        printf("file can't be opened \n");
    }

    graph_t * graph;

    graph= graph_read_from_file(file);

    
}