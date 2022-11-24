#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "graph.h"
#include "array.h"


int main(int argc, char* argv[]){
    array_int *a = array_int_init(10);
    for(int i=0; i<10; i++)
        array_int_push(a,i);
    array_int_print(a);
    array_int_clear(a);
    array_int_print(a);
    array_int_free(a);
}