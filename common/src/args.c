#include "args.h"
#include <stdlib.h>

main_parameters_t get_input (int argc, char *argv[]){
    main_parameters_t main;
    if(argc <= 1){
        main.t=INPUT_ERROR;
        return main;
    }
    char first_param[100]; 
    char second_param[100];
    
    sscanf(argv[1],"%s",first_param);
    
    if((strcmp(first_param,"-sm")) == 0){
        if(argc >= 3 ){
            sscanf(argv[2],"%s",second_param);
            main.t=INPUT_TYPE_SHMEM;
            strcpy(main.name,second_param);
        } else{
            printf("\nfor select the read graph from shared memory insert id: \n");
            exit(1);
        }
    }else{
        main.t= INPUT_TYPE_FILE;
        strcpy(main.name,first_param);
    }

    return main;
}