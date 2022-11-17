#include "args.h"

int main(int argc, char* argv[]){
    printf("start del mio test")
    main_parameters_t c;
    c= get_input(argc, argv);
    printf("file:%s\n enum %s", c.name, c.t);
}