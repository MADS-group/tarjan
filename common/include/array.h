#ifndef ARRAY_H
#define ARRAY_H

typedef struct array_int array_int;

array_int *array_int_init(int length); //Array initialization
void array_int_free(array_int *a); //Array deallocation
void array_int_resize(array_int *a, int length); //Array resizing. Length is the new length of the array.
void array_int_push(array_int *a, int elem); //Adds an element at the end of the array
int array_int_pop(array_int *a); //Pops last element of the array
int array_int_get(array_int *a, int pos); //Gets element at pos
int array_int_length(array_int *a); //Gets number of elements in the array
void array_int_set(array_int *a, int pos, int elem); //Sets element at pos
void array_int_print(array_int *a); //Prints all the elements of the array

typedef struct array_ptr array_ptr;

array_ptr *array_ptr_init(int length);
void array_ptr_free(array_ptr *a);
void array_ptr_resize(array_ptr *a, int length);
void array_ptr_push(array_ptr *a, void *elem); //Adds an element at the end of the array
void *array_ptr_pop(array_ptr *a); //Pops last element of the array
void *array_ptr_get(array_ptr *a, int pos); //Gets element at pos
int array_ptr_length(array_ptr *a); //Gets number of elements in the array
void array_ptr_set(array_ptr *a, int pos, void *elem); //Sets element at pos
void array_ptr_print(array_ptr *a); //Prints all the elements of the array

#endif