#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"
#include <assert.h>
struct lnode_int_t{
    int info;
    lnode_int_t* nextLink;
    lnode_int_t* prevLink;
};

struct linkedlist_int{
    lnode_int_t* FirstNode; // puntatore al primo nodo
    lnode_int_t* LastNode; // puntatore all'ultimo nodo
    int length;   // lunghezza array    
};


lnode_int_t *nodeCreate(int info) {
    lnode_int_t *new = malloc (sizeof(lnode_int_t));
    if(new == NULL){
        printf("Error. Node not allocate\n");
        exit(1);
    }
    if (new != NULL) {
        new->info = info;
        new->nextLink = NULL;
        new->prevLink = NULL;
    }
    return new;
}

void nodeDestroy(lnode_int_t* n) {
    free(n);
}

linkedlist_int *linkedlist_int_init(){
    //linkedlist initialization
    linkedlist_int* l = malloc(sizeof(linkedlist_int));
    l->length = 0;
    l->FirstNode = NULL;
    l->LastNode = NULL;
    return l;
}

void linkedlist_int_free(linkedlist_int *a){
    //linkedlist deallocation
    lnode_int_t *last = a->LastNode;
    lnode_int_t *node = a->LastNode;
    while(node != NULL){
        node = last->prevLink;
        nodeDestroy(last);
        last = node;
    }
}

int linkedlist_int_length(linkedlist_int *a){
    //Gets number of elements in the linkedlist
    return a->length;
} 
void linkedlist_int_push(linkedlist_int *a, int elem){
    //Adds an element at the end of the linkedlist
    if(a == NULL){
        printf("Error. Invalid LinkedList");
        exit(1);
    }
    lnode_int_t * new = nodeCreate(elem);
    //lnode_int_t* n = a->LastNode;
    if(a->length == 0){
        //Nel caso in cui la linkedlist è vuota
        a->LastNode = new;
        a->FirstNode = new;
    }else{
        //Nel caso in cui la linkedlist NON è vuota
        lnode_int_t* last = a->LastNode;
        new->prevLink = last;
        last->nextLink = new;
        a->LastNode = new;
    }
    a->length +=1;
    return;
}

int linkedlist_int_pop(linkedlist_int *a){
    //Pops last element of the linkedlist
    if(a == NULL){
        printf("Error. Invalid LinkedList\n");
        exit(1);
    }
    if(a->length == 0){
       printf("Error list is empty\n"); 
       exit(1);
    }else if(a->length == 1){
        //La lista ha un solo elemento.
        lnode_int_t* last_node = a->LastNode;
        int inf = last_node->info;
        a->FirstNode = NULL;
        a->LastNode = NULL;
        a->length -=1;
        nodeDestroy(last_node);
        return inf;
    }else{
        //La lista ha più di un elemento.
        lnode_int_t* last_node = a->LastNode;
        int inf = last_node->info;
        lnode_int_t* penultimo_node = last_node->prevLink;
        penultimo_node->nextLink = NULL;
        a->LastNode = penultimo_node;
        a->length -=1;
        nodeDestroy(last_node);
        return inf;
    }
} 

void linkedlist_int_insert(linkedlist_int *a, int elem){
    //Insert the element in order
    if(a == NULL){
        printf("Error. Invalid LinkedList\n");
        exit(1);
    }

    //Se la lista è vuota  
    if(a->length == 0){
        lnode_int_t *new = nodeCreate(elem); //inizializza a NULL i puntatori del nodo
        a->FirstNode = new;
        a->LastNode = new;
        a->length +=1;
        return;
    }

    //Se la lista NON è vuota
    int isFirstElement = 1;
    lnode_int_t* node =  a->FirstNode;
    while(elem > node->info){
        isFirstElement = 0;
        if(node->nextLink == NULL){
            //Sono arrivato alla fine della lista
            lnode_int_t *new = nodeCreate(elem);
            new->nextLink = NULL;
            new->prevLink = node;
            node->nextLink = new;
            a->LastNode = new;
            a->length +=1;
            return;
        }
        node = node->nextLink;
    }
    
    lnode_int_t *new = nodeCreate(elem);
    if(isFirstElement == 1){
        //L'inserimento deve avvenire in testa
        a->FirstNode = new;
        new->nextLink = node;
        node->prevLink = new;
        a->length +=1;
        return;
    }
    //La lista non è vuota e l'inserimento deve avvenire nel mezzo (Non in testa e non in coda) 
    lnode_int_t * prev = node->prevLink;
    prev->nextLink = new;
    node->prevLink = new;
    new->nextLink = node;
    new->prevLink = prev;
    a->length +=1;
    return;

}
void linkedlist_int_delete(linkedlist_int *a, int elem){
    //Delete the element if it exists
    if(a == NULL){
        printf("Error. Invalid LinkedList\n");
        exit(1);
    }
    //Se la lista è vuota  
    if(a->length == 0){
        return;
    }

    //La lista ha un solo elemento
    if(a->length == 1){

        //Se l'elemento è presente nella lista allora viene cancellato 
        if(a->FirstNode->info == elem){
            nodeDestroy(a->FirstNode);
            a->length -=1;
            a->FirstNode = NULL;
            a->LastNode = NULL;
        }
        //Se l'elemento non è presente non viene fatto niente 
        return;
    }

    //La lista ha più di un elemento
    int isFirstElement = 1;
    lnode_int_t* node =  a->FirstNode;
    while(elem != node->info){
        isFirstElement = 0;
        if(node->nextLink == NULL){
            //FINITO DI SCORRERE LA LISTA E IL NODO NON È STATO TROVATO
            return;
        }
        node = node->nextLink;
    }
    if(isFirstElement == 1){
        //DEVO CANCELLARE IL PRIMO ELEMENTO
        a->FirstNode = node->nextLink;
        a->FirstNode->prevLink = NULL;
        nodeDestroy(node);
        a->length -=1;
        return;
    }
    if(node == a->LastNode){
        //DEVO CANCELLARE L'ULTIMO ELEMENTO
        lnode_int_t *prev = node->prevLink;
        prev->nextLink = NULL;
        a->LastNode = prev;
        nodeDestroy(node);
        a->length -=1;
        return;
    }
    //Devo cancellare un nodo nel mezzo (Non in testa e non in coda)
    lnode_int_t *prev= node->prevLink; 
    lnode_int_t *next= node->nextLink;
    prev->nextLink = next;
    next->prevLink = prev;
    nodeDestroy(node);
    a->length -=1;
    return;

} 

void linkedlist_int_enqueue(linkedlist_int *a, int elem){
    //enqueue an element at the end of the linkedlist
    if(a == NULL){
        printf("Error. Invalid LinkedList");
        exit(1);
    }
    lnode_int_t * new = nodeCreate(elem);
    if(a->length == 0){
        //Nel caso in cui la linkedlist è vuota
        a->LastNode = new;
        a->FirstNode = new;
    }else{
        //Nel caso in cui la linkedlist NON è vuota
        lnode_int_t* first = a->FirstNode;
        new->nextLink = first;
        first->prevLink = new;
        a->FirstNode = new;
    }
    a->length +=1;
    return;

}
int linkedlist_int_dequeue(linkedlist_int *a){
    //dequeue the last item of the linkedlist
    
    if(a == NULL){
        printf("Error. Invalid LinkedList\n");
        exit(1);
    }
    if(a->length == 0){
       printf("Error! List is empty\n"); 
       exit(1);
    }else if(a->length == 1){
        //La lista ha un solo elemento.
        lnode_int_t* last_node = a->LastNode;
        int inf = last_node->info;
        a->FirstNode = NULL;
        a->LastNode = NULL;
        a->length -=1;
        nodeDestroy(last_node);
        return inf;
    }else{
        //La lista ha più di un elemento.
        lnode_int_t* last_node = a->LastNode;
        int inf = last_node->info;
        lnode_int_t* penultimo_node = last_node->prevLink;
        penultimo_node->nextLink = NULL;
        a->LastNode = penultimo_node;
        a->length -=1;
        nodeDestroy(last_node);
        return inf;
    }
} 
void linkedlist_int_print(linkedlist_int *a){
    //Prints all the elements of the linkedlist
    lnode_int_t* l = a->FirstNode;
    printf ("PRINT DEBUG: \n");
    printf ("Numero nodi: %d\n",a->length);
    while (l != NULL) {
        printf("%d ",l->info);
        l = l->nextLink;
    }
    printf ("\nFINE DEBUG\n");
    
} 

int linkedlist_int_top(linkedlist_int *a){
    assert(a->LastNode != NULL);
    return a->LastNode->info;
}

void linkedlist_int_cpy(linkedlist_int *to,linkedlist_int *from){
    //Copy all the elements from a linkedlist to another

}


typedef struct linkedlist_ptr linkedlist_ptr;


/*
lnode_int_t *nodeCreate(int info) {
    lnode_int_t *new = malloc (sizeof(lnode_int_t));
    if(new == NULL){
        printf("Error. Node not allocate\n");
        exit(1);
    }
    if (new != NULL) {
        new->info = info;
        new->nextLink = NULL;
        new->prevLink = NULL;
    }
    return new;
}

linkedlist_ptr *linkedlist_ptr_init(){
    //linkedlist initialization

}
void linkedlist_ptr_free(linkedlist_ptr *a){
    //linkedlist deallocation

}
int linkedlist_ptr_length(linkedlist_ptr *a){
    
} 
void linkedlist_ptr_push(linkedlist_ptr *a, void *elem){
    //Adds an element at the end of the linkedlist
}
void* linkedlist_ptr_pop(linkedlist_ptr *a){
    //Pops last element of the linkedlist
}

int linkedlist_ptr_insert(linkedlist_ptr *a, void *elem){
    //Innsert the element in order

}
void linkedlist_ptr_delete(linkedlist_ptr *a, void *elem){
     //Delete the element if it exists
}
void linkedlist_ptr_enqueue(linkedlist_ptr *a, void *elem) {
    //enqueue an element at the end of the linkedlist
}

void* linkedlist_ptr_dequeue(linkedlist_ptr *a){
    //dequeue of the linkedlist
}
void linkedlist_ptr_print(linkedlist_ptr *a) {
    //Prints all the elements of the linkedlist
}
void linkedlist_ptr_cpy(linkedlist_ptr *to,linkedlist_ptr *from){
    //Copy all the elements from a linkedlist to another
}



void nodeDestroy(lnode_int_t* n) {
    free(n);
}

linkedlist_int *linkedlist_int_init(){
    //linkedlist initialization
    linkedlist_int* l = malloc(sizeof(linkedlist_int));
    l->length = 0;
    l->FirstNode = NULL;
    l->LastNode = NULL;
    return l;
}

void linkedlist_int_free(linkedlist_int *a){
    //linkedlist deallocation
    lnode_int_t *last = a->LastNode;
    lnode_int_t *node = a->LastNode;
    while(node != NULL){
        node = last->prevLink;
        nodeDestroy(last);
        last = node;
    }
}

int linkedlist_int_length(linkedlist_int *a){
    //Gets number of elements in the linkedlist
    return a->length;
} 
void linkedlist_int_push(linkedlist_int *a, int elem){
    //Adds an element at the end of the linkedlist
    if(a == NULL){
        printf("Error. Invalid LinkedList");
        exit(1);
    }
    lnode_int_t * new = nodeCreate(elem);
    //lnode_int_t* n = a->LastNode;
    if(a->length == 0){
        //Nel caso in cui la linkedlist è vuota
        a->LastNode = new;
        a->FirstNode = new;
    }else{
        //Nel caso in cui la linkedlist NON è vuota
        lnode_int_t* last = a->LastNode;
        new->prevLink = last;
        last->nextLink = new;
        a->LastNode = new;
    }
    a->length +=1;
    return;
}

int linkedlist_int_pop(linkedlist_int *a){
    //Pops last element of the linkedlist
    if(a == NULL){
        printf("Error. Invalid LinkedList\n");
        exit(1);
    }
    if(a->length == 0){
       printf("Error list is empty\n"); 
       exit(1);
    }else if(a->length == 1){
        //La lista ha un solo elemento.
        lnode_int_t* last_node = a->LastNode;
        int inf = last_node->info;
        a->FirstNode = NULL;
        a->LastNode = NULL;
        a->length -=1;
        nodeDestroy(last_node);
        return inf;
    }else{
        //La lista ha più di un elemento.
        lnode_int_t* last_node = a->LastNode;
        int inf = last_node->info;
        lnode_int_t* penultimo_node = last_node->prevLink;
        penultimo_node->nextLink = NULL;
        a->LastNode = penultimo_node;
        a->length -=1;
        nodeDestroy(last_node);
        return inf;
    }
} 

void linkedlist_int_insert(linkedlist_int *a, int elem){
    //Insert the element in order
    if(a == NULL){
        printf("Error. Invalid LinkedList\n");
        exit(1);
    }

    //Se la lista è vuota  
    if(a->length == 0){
        lnode_int_t *new = nodeCreate(elem); //inizializza a NULL i puntatori del nodo
        a->FirstNode = new;
        a->LastNode = new;
        a->length +=1;
        return;
    }

    //Se la lista NON è vuota
    int isFirstElement = 1;
    lnode_int_t* node =  a->FirstNode;
    while(elem > node->info){
        isFirstElement = 0;
        if(node->nextLink == NULL){
            //Sono arrivato alla fine della lista
            lnode_int_t *new = nodeCreate(elem);
            new->nextLink = NULL;
            new->prevLink = node;
            node->nextLink = new;
            a->LastNode = new;
            a->length +=1;
            return;
        }
        node = node->nextLink;
    }
    
    lnode_int_t *new = nodeCreate(elem);
    if(isFirstElement == 1){
        //L'inserimento deve avvenire in testa
        a->FirstNode = new;
        new->nextLink = node;
        node->prevLink = new;
        a->length +=1;
        return;
    }
    //La lista non è vuota e l'inserimento deve avvenire nel mezzo (Non in testa e non in coda) 
    lnode_int_t * prev = node->prevLink;
    prev->nextLink = new;
    node->prevLink = new;
    new->nextLink = node;
    new->prevLink = prev;
    a->length +=1;
    return;

}
void linkedlist_int_delete(linkedlist_int *a, int elem){
    //Delete the element if it exists
    if(a == NULL){
        printf("Error. Invalid LinkedList\n");
        exit(1);
    }
    //Se la lista è vuota  
    if(a->length == 0){
        return;
    }

    //La lista ha un solo elemento
    if(a->length == 1){

        //Se l'elemento è presente nella lista allora viene cancellato 
        if(a->FirstNode->info == elem){
            nodeDestroy(a->FirstNode);
            a->length -=1;
            a->FirstNode = NULL;
            a->LastNode = NULL;
        }
        //Se l'elemento non è presente non viene fatto niente 
        return;
    }

    //La lista ha più di un elemento
    int isFirstElement = 1;
    lnode_int_t* node =  a->FirstNode;
    while(elem != node->info){
        isFirstElement = 0;
        if(node->nextLink == NULL){
            //FINITO DI SCORRERE LA LISTA E IL NODO NON È STATO TROVATO
            return;
        }
        node = node->nextLink;
    }
    if(isFirstElement == 1){
        //DEVO CANCELLARE IL PRIMO ELEMENTO
        a->FirstNode = node->nextLink;
        a->FirstNode->prevLink = NULL;
        nodeDestroy(node);
        a->length -=1;
        return;
    }
    if(node == a->LastNode){
        //DEVO CANCELLARE L'ULTIMO ELEMENTO
        lnode_int_t *prev = node->prevLink;
        prev->nextLink = NULL;
        a->LastNode = prev;
        nodeDestroy(node);
        a->length -=1;
        return;
    }
    //Devo cancellare un nodo nel mezzo (Non in testa e non in coda)
    lnode_int_t *prev= node->prevLink; 
    lnode_int_t *next= node->nextLink;
    prev->nextLink = next;
    next->prevLink = prev;
    nodeDestroy(node);
    a->length -=1;
    return;

} 

void linkedlist_int_enqueue(linkedlist_int *a, int elem){
    //enqueue an element at the end of the linkedlist
    if(a == NULL){
        printf("Error. Invalid LinkedList");
        exit(1);
    }
    lnode_int_t * new = nodeCreate(elem);
    if(a->length == 0){
        //Nel caso in cui la linkedlist è vuota
        a->LastNode = new;
        a->FirstNode = new;
    }else{
        //Nel caso in cui la linkedlist NON è vuota
        lnode_int_t* first = a->FirstNode;
        new->nextLink = first;
        first->prevLink = new;
        a->FirstNode = new;
    }
    a->length +=1;
    return;

}
int linkedlist_int_dequeue(linkedlist_int *a){
    //dequeue the last item of the linkedlist
    
    if(a == NULL){
        printf("Error. Invalid LinkedList\n");
        exit(1);
    }
    if(a->length == 0){
       printf("Error! List is empty\n"); 
       exit(1);
    }else if(a->length == 1){
        //La lista ha un solo elemento.
        lnode_int_t* last_node = a->LastNode;
        int inf = last_node->info;
        a->FirstNode = NULL;
        a->LastNode = NULL;
        a->length -=1;
        nodeDestroy(last_node);
        return inf;
    }else{
        //La lista ha più di un elemento.
        lnode_int_t* last_node = a->LastNode;
        int inf = last_node->info;
        lnode_int_t* penultimo_node = last_node->prevLink;
        penultimo_node->nextLink = NULL;
        a->LastNode = penultimo_node;
        a->length -=1;
        nodeDestroy(last_node);
        return inf;
    }
} 
void linkedlist_int_print(linkedlist_int *a){
    //Prints all the elements of the linkedlist
    lnode_int_t* l = a->FirstNode;
    printf ("PRINT DEBUG: \n");
    printf ("Numero nodi: %d\n",a->length);
    while (l != NULL) {
        printf("%d ",l->info);
        l = l->nextLink;
    }
    printf ("\nFINE DEBUG\n");
    
} 
void linkedlist_int_cpy(linkedlist_int *to,linkedlist_int *from){
    //Copy all the elements from a linkedlist to another

}*/