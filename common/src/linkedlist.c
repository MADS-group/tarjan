/* 
 * Course: High Performance Computing 2022/2023 
 * 
 * Lecturer: Francesco Moscato  fmoscato@unisa.it 
 *
 * Group:
 * Langella	Antonio   0622702011   a.langella31@studenti.unisa.it
 * Marsico Michele   0622702012   m.marsico10@studenti.unisa.it
 * Paolino Salvatore   0622702016   s.paolino6@studenti.unisa.it 
 * Risi Davide   0622702013   d.risi2@studenti.unisa.it
 * 
 * Copyright (C) 2023 - All Rights Reserved 
 *
 * This file is part of Parallelize-Tarjan-MPI-CUDA. 
 *
 * Parallelize-Tarjan-MPI-CUDA is free software: you can redistribute it and/or modify 
 * it under the terms of the GNU General Public License as published by 
 * the Free Software Foundation, either version 3 of the License, or 
 * (at your option) any later version. 
 *
 * Parallelize-Tarjan-MPI-CUDA is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * GNU General Public License for more details. 
 *
 * You should have received a copy of the GNU General Public License 
 * along with Parallelize-Tarjan-MPI-CUDA.  If not, see <http://www.gnu.org/licenses/>. 
 * 
 */

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
    lnode_int_t *new_elem = (lnode_int_t *) malloc (sizeof(lnode_int_t));
    if(new_elem == NULL){
        printf("Error. Node not allocate\n");
        exit(1);
    }
    if (new_elem != NULL) {
        new_elem->info = info;
        new_elem->nextLink = NULL;
        new_elem->prevLink = NULL;
    }
    return new_elem;
}

void nodeDestroy(lnode_int_t* n) {
    free(n);
}

linkedlist_int *linkedlist_int_init(){
    //linkedlist initialization
    linkedlist_int* l = (linkedlist_int *) malloc(sizeof(linkedlist_int));
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
    lnode_int_t * new_elem = nodeCreate(elem);
    //lnode_int_t* n = a->LastNode;
    if(a->length == 0){
        //Nel caso in cui la linkedlist è vuota
        a->LastNode = new_elem;
        a->FirstNode = new_elem;
    }else{
        //Nel caso in cui la linkedlist NON è vuota
        lnode_int_t* last = a->LastNode;
        new_elem->prevLink = last;
        last->nextLink = new_elem;
        a->LastNode = new_elem;
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
        lnode_int_t *new_elem = nodeCreate(elem); //inizializza a NULL i puntatori del nodo
        a->FirstNode = new_elem;
        a->LastNode = new_elem;
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
            lnode_int_t *new_elem = nodeCreate(elem);
            new_elem->nextLink = NULL;
            new_elem->prevLink = node;
            node->nextLink = new_elem;
            a->LastNode = new_elem;
            a->length +=1;
            return;
        }
        node = node->nextLink;
    }
    
    lnode_int_t *new_elem = nodeCreate(elem);
    if(isFirstElement == 1){
        //L'inserimento deve avvenire in testa
        a->FirstNode = new_elem;
        new_elem->nextLink = node;
        node->prevLink = new_elem;
        a->length +=1;
        return;
    }
    //La lista non è vuota e l'inserimento deve avvenire nel mezzo (Non in testa e non in coda) 
    lnode_int_t * prev = node->prevLink;
    prev->nextLink = new_elem;
    node->prevLink = new_elem;
    new_elem->nextLink = node;
    new_elem->prevLink = prev;
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
    lnode_int_t * new_elem = nodeCreate(elem);
    if(a->length == 0){
        //Nel caso in cui la linkedlist è vuota
        a->LastNode = new_elem;
        a->FirstNode = new_elem;
    }else{
        //Nel caso in cui la linkedlist NON è vuota
        lnode_int_t* first = a->FirstNode;
        new_elem->nextLink = first;
        first->prevLink = new_elem;
        a->FirstNode = new_elem;
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
    lnode_int_t *new_elem = malloc (sizeof(lnode_int_t));
    if(new_elem == NULL){
        printf("Error. Node not allocate\n");
        exit(1);
    }
    if (new_elem != NULL) {
        new_elem->info = info;
        new_elem->nextLink = NULL;
        new_elem->prevLink = NULL;
    }
    return new_elem;
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
    lnode_int_t * new_elem = nodeCreate(elem);
    //lnode_int_t* n = a->LastNode;
    if(a->length == 0){
        //Nel caso in cui la linkedlist è vuota
        a->LastNode = new_elem;
        a->FirstNode = new_elem;
    }else{
        //Nel caso in cui la linkedlist NON è vuota
        lnode_int_t* last = a->LastNode;
        new_elem->prevLink = last;
        last->nextLink = new_elem;
        a->LastNode = new_elem;
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
        lnode_int_t *new_elem = nodeCreate(elem); //inizializza a NULL i puntatori del nodo
        a->FirstNode = new_elem;
        a->LastNode = new_elem;
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
            lnode_int_t *new_elem = nodeCreate(elem);
            new_elem->nextLink = NULL;
            new_elem->prevLink = node;
            node->nextLink = new_elem;
            a->LastNode = new_elem;
            a->length +=1;
            return;
        }
        node = node->nextLink;
    }
    
    lnode_int_t *new_elem = nodeCreate(elem);
    if(isFirstElement == 1){
        //L'inserimento deve avvenire in testa
        a->FirstNode = new_elem;
        new_elem->nextLink = node;
        node->prevLink = new_elem;
        a->length +=1;
        return;
    }
    //La lista non è vuota e l'inserimento deve avvenire nel mezzo (Non in testa e non in coda) 
    lnode_int_t * prev = node->prevLink;
    prev->nextLink = new_elem;
    node->prevLink = new_elem;
    new_elem->nextLink = node;
    new_elem->prevLink = prev;
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
    lnode_int_t * new_elem = nodeCreate(elem);
    if(a->length == 0){
        //Nel caso in cui la linkedlist è vuota
        a->LastNode = new_elem;
        a->FirstNode = new_elem;
    }else{
        //Nel caso in cui la linkedlist NON è vuota
        lnode_int_t* first = a->FirstNode;
        new_elem->nextLink = first;
        first->prevLink = new_elem;
        a->FirstNode = new_elem;
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