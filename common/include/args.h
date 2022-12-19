#ifndef ARGS_H
#define ARGS_H

#include <stdio.h>  
#include <string.h>

typedef enum {
    INPUT_TYPE_FILE,
    INPUT_TYPE_SHMEM,
    INPUT_ERROR
} input_t;

typedef struct{
    char first_param[100];
    char second_param[100];
    input_t t;
} main_parameters_t;

main_parameters_t get_input(int argc, char *argv[]);
#endif
