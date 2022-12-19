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
    sscanf(argv[2],"%s",second_param);

    
    
    main.t= INPUT_TYPE_FILE;
    strcpy(main.first_param, first_param);
    strcpy(main.second_param, second_param);

    return main;
}