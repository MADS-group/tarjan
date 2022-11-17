
main_parameters_t get_input(int argc, char *argv[]){
    main_parameters_t main;
    if(argc <= 1){
        printf("\n incorrent parameters:
                \n sequential.out filename -> read graph from file named filename
                \nsequential.out -sm id -> read graph fromm shared memory with identifier id");
        exit(1);
    }
    char first_param[100]; 
    char second_param[100];
    
    sscanf(argv[1],"%s",first_param);
    
    if((strcmp(first_param,"-sm")) == 0){
        if(argc >= 3 ){
            sscanf(argv[1],"%s",second_param);
            main.t=input_t.INPUT_TYPE_SHMEM;
            main.name=second_param;
        } else{
            printf("\nfor select the read graph from shared memory insert id: \n")
            exit(1);
        }
    }else{
        main.t=input_t.INPUT_TYPE_FILE;
        main.name=first_param;
    }

    return (main)
}