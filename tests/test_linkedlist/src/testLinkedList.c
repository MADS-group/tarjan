#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"

int main(){
    linkedlist_int* ll= linkedlist_int_init();
    /*
    linkedlist_int_push(ll,1);
    printf("n: %d\n",linkedlist_int_length(ll));
    linkedlist_int_push(ll,2);
    printf("n: %d\n",linkedlist_int_length(ll));
    linkedlist_int_push(ll,3);
    printf("n: %d\n",linkedlist_int_length(ll));
    linkedlist_int_push(ll,4);
    linkedlist_int_push(ll,5);
    linkedlist_int_push(ll,6);
    linkedlist_int_push(ll,7);
    linkedlist_int_push(ll,8);
    linkedlist_int_push(ll,9);
    printf("%d\n",linkedlist_int_length(ll));
    linkedlist_int_print(ll);

    printf("%d ",linkedlist_int_pop(ll));
    printf("%d ",linkedlist_int_pop(ll));
    printf("%d ",linkedlist_int_pop(ll));
    printf("%d ",linkedlist_int_pop(ll));
    printf("%d ",linkedlist_int_pop(ll));
    printf("%d ",linkedlist_int_pop(ll));
    printf("%d ",linkedlist_int_pop(ll));
    printf("%d ",linkedlist_int_pop(ll));
    printf("%d ",linkedlist_int_pop(ll));
    printf("\nn: %d\n",linkedlist_int_length(ll));

    //printf("\nMi aspetto un'eccezione\n");
    //printf("%d",linkedlist_int_pop(ll));

    */
   /*
    linkedlist_int_insert(ll,1);
    linkedlist_int_insert(ll,5);
    linkedlist_int_insert(ll,10);
    printf("n: %d\n",linkedlist_int_length(ll));
    linkedlist_int_insert(ll,3);
    linkedlist_int_insert(ll,8);
    linkedlist_int_insert(ll,2);
    printf("n: %d\n",linkedlist_int_length(ll));
    linkedlist_int_insert(ll,0);
    printf("n: %d\n",linkedlist_int_length(ll));
    linkedlist_int_print(ll);

    linkedlist_int_delete(ll,1);
    linkedlist_int_print(ll);
    linkedlist_int_delete(ll,0);
    linkedlist_int_print(ll);
    linkedlist_int_delete(ll,10);
    linkedlist_int_print(ll);

    linkedlist_int_free(ll);
    */
    linkedlist_int_enqueue(ll,1);
    printf("n: %d\n",linkedlist_int_length(ll));
    linkedlist_int_enqueue(ll,2);
    printf("n: %d\n",linkedlist_int_length(ll));
    linkedlist_int_enqueue(ll,3);
    printf("n: %d\n",linkedlist_int_length(ll));
    linkedlist_int_enqueue(ll,4);
    linkedlist_int_enqueue(ll,5);
    linkedlist_int_enqueue(ll,6);
    linkedlist_int_enqueue(ll,7);
    linkedlist_int_enqueue(ll,8);
    linkedlist_int_enqueue(ll,9);
    printf("%d\n",linkedlist_int_length(ll));
    linkedlist_int_print(ll);

    printf("%d ",linkedlist_int_dequeue(ll));
    printf("%d ",linkedlist_int_dequeue(ll));
    printf("%d ",linkedlist_int_dequeue(ll));
    printf("%d ",linkedlist_int_dequeue(ll));
    printf("%d ",linkedlist_int_dequeue(ll));
    printf("%d ",linkedlist_int_dequeue(ll));
    printf("%d ",linkedlist_int_dequeue(ll));
    printf("%d ",linkedlist_int_dequeue(ll));
    printf("%d ",linkedlist_int_dequeue(ll));
    printf("\nn: %d\n",linkedlist_int_length(ll));
    linkedlist_int_print(ll);

    linkedlist_int_free(ll);
    return 0;
}