#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "graph.h"
#include "array.h"

void fun(array_int *scc){
    array_int_print(scc);
}

void fun2(void *local(array_int *)){
    array_int * a = array_int_init(10);
    array_int_push(a, 10);
    array_int_push(a, 9);
    array_int_push(a, 8);
    array_int_push(a, 7);
    array_int_push(a, 6);
    printf("fun2()\n");
    local(a);
}

int main(int argc, char* argv[]){
    void (*fun_ptr)(array_int *) = &fun;
    fun2(fun_ptr);
}