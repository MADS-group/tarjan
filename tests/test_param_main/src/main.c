#include "args.h"
#include <stdlib.h>

int main(int argc, char* argv[]){
    main_parameters_t c;
    c= get_input(argc, argv);

    switch (c.t){
        case INPUT_ERROR:
            printf("incorrent parameters:\n sequential.out filename -> read graph from file named filename\n sequential.out -sm id -> read graph fromm shared memory with identifier id\n");
            exit(1);
            break;
        case INPUT_TYPE_FILE:
            printf("file name:%s, enum INPUT_TYPE_FILE\n", c.first_param);
            break;
    }

}