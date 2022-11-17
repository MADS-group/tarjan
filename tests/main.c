#include "args.h"

int main(int argc, char* argv[]){
    printf("start del mio test\n");
    main_parameters_t c;
    c= get_input(argc, argv);

    switch (c.t){
        case INPUT_ERROR:
            printf("\n incorrent parameters:\n sequential.out filename -> read graph from file named filename\nsequential.out -sm id -> read graph fromm shared memory with identifier id\n");
            exit(1);
            break;
        case INPUT_TYPE_FILE:
            printf("file name:%s, enum %d\n", c.name, c.t);
            break;
        case INPUT_TYPE_SHMEM:
            printf("id shared:%s, enum %d\n", c.name, c.t);
            break;
    }
    
}