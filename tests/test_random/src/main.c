#include <stdlib.h>
#include <stdio.h>
#include "random.h"


int main(int argc, char* argv[]){
    (void) argc;
    (void) argv;
    
    for(int i=0; i<100; i++){
        printf("%d ",rand_binomial_2(10,1));
    }
    printf("\n");
}