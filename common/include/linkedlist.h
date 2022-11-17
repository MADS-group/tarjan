#ifndef LINKEDLIST_H
#define LINKEDLIST_H

typedef struct linkedlist_int linkedlist_int;

linkedlist_int *linkedlist_int_init(); //linkedlist initialization
void linkedlist_int_free(linkedlist_int *a); //linkedlist deallocation
int linkedlist_int_length(linkedlist_int *a); //Gets number of elements in the linkedlist
void linkedlist_int_push(linkedlist_int *a, int elem); //Adds an element at the end of the linkedlist
int linkedlist_int_pop(linkedlist_int *a); //Pops last element of the linkedlist
int linkedlist_int_insert(linkedlist_int *a, int elem); //Innsert the element in order
void linkedlist_int_delete(linkedlist_int *a, int elem); //Delete the element if it exists
void linkedlist_int_enqueue(linkedlist_int *a, int elem); //enqueue an element at the end of the linkedlist
int linkedlist_int_dequeue(linkedlist_int *a); //dequeue of the linkedlist
void linkedlist_int_print(linkedlist_int *a); //Prints all the elements of the linkedlist
void linkedlist_int_cpy(linkedlist_int *to,linkedlist_int *from); //Copy all the elements from a linkedlist to another


typedef struct linkedlist_ptr linkedlist_ptr;

linkedlist_ptr *linkedlist_ptr_init(); //linkedlist initialization
void linkedlist_ptr_free(linkedlist_ptr *a); //linkedlist deallocation
int linkedlist_ptr_length(linkedlist_ptr *a); //Gets number of elements in the linkedlist
void linkedlist_ptr_push(linkedlist_ptr *a, void *elem); //Adds an element at the end of the linkedlist
void* linkedlist_ptr_pop(linkedlist_ptr *a); //Pops last element of the linkedlist
int linkedlist_ptr_insert(linkedlist_ptr *a, void *elem); //Innsert the element in order
void linkedlist_ptr_delete(linkedlist_ptr *a, void *elem); //Delete the element if it exists
void linkedlist_ptr_enqueue(linkedlist_ptr *a, void *elem); //enqueue an element at the end of the linkedlist
void* linkedlist_ptr_dequeue(linkedlist_ptr *a); //dequeue of the linkedlist
void linkedlist_ptr_print(linkedlist_ptr *a); //Prints all the elements of the linkedlist
void linkedlist_ptr_cpy(linkedlist_ptr *to,linkedlist_ptr *from); //Copy all the elements from a linkedlist to another


#endif