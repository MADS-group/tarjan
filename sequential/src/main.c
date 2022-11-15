#include <stdlib.h>
#include <stdio.h>
#include "graph.h"
#include "array.h"

int main(int argc, char* argv[]){
    FILE* file;
    file = fopen("test.txt", "r");

    if (NULL == file) {
        printf("file can't be opened \n");
    }

    graph_t * graph;
    graph= graph_init(); //aggiungere n
    graph= graph_read_from_file(file);

    array result;
    result= arrayCreate(); //aggiungere lethg
    result= *(graph_tarjan(graph));

}