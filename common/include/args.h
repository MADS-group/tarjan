#ifndef ARGS_H
#define ARGS_H

#include <stdio.h>  
#include<string.h>

typedef enum {
    INPUT_TYPE_FILE,
    INPUT_TYPE_SHMEM
} input_t;

typedef struct{
    char name[100];
    input_t t;
} main_parameters_t;

main_parameters_t get_input(int argc, char *argv[]);

