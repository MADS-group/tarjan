//Dynamic array implementation
#include "array.h"
#include <stdlib.h>
#include <assert.h>

struct array_int {
    int *items; // puntatore agli elementi dell'array
    int length;   // lunghezza array    
    int size;     // dimensione allocata (>= length)
    int C_EXP;   // costante di espansione
    int C_RED;   // costante di riduzione
};

array_int *array_int_init(int length) {
    array_int *a;
    a = malloc(sizeof(array_int));
    a->C_EXP=2;
    a->C_RED=3;
    int size = length * a->C_EXP;
    a->items = malloc(size * sizeof(int));
    assert (size == 0 || a->items != NULL); 
    a->length = length;
    a->size = size; 
    //printf ("Creazione array: length = %d, size = %d\n", length, size);
    return a;
}

void array_int_free(array_int *a) {
    free(a->items);
    a->items = NULL;
    a->length = a->size = 0;
    free(a);
}

void array_int_resize(array_int *a, int length) {
    if (length > a->size || length < 1.0 * a->size / a->C_RED) {
        int size = length * a->C_EXP;
        a->items = realloc(a->items, size * sizeof(int));
        assert (size == 0 || a->items != NULL); 
        a->size = size;        
    }
    a->length = length;
    //printf ("Ridimensionamento array: length = %d, size = %d\n", length, a->size);
 }

// Aggiunta di un elemento in coda all'array
void array_int_push(array_int *a, int elem) {
    array_int_resize(a, a->length + 1);
    a->items[a->length - 1] = elem;
}
// Pops last element of the array
int array_int_pop(array_int *a){
    int item = a->items[a->length - 1];
    array_int_resize(a, a->length - 1);
    return item;
}
//Lettura di un elemento in posizione pos
int array_int_get(array_int *a, int pos) {
    return a->items[pos];
}

// Lettura della lunghezza dell'array
int array_int_length(array_int *a) {
    return a->length;
}

// Inserimento di un elemento in posizione pos con ridimensionamento automatico
void array_int_set(array_int *a, int pos, int elem) {
    if (pos >= a->length)
        array_int_resize(a, pos + 1);
    a->items[pos] = elem;
}

// Stampa dell'array
void array_int_print(array_int *a) {
    for (int i = 0; i < a->length; ++i)
        printf ("%d ", a->items[i]);
    printf ("\n");
}

//TODO: Aggiungere implementazione array_ptr

struct array_ptr {
    int *items; // puntatore agli elementi dell'array
    int length;   // lunghezza array    
    int size;     // dimensione allocata (>= length)
    int C_EXP;   // costante di espansione
    int C_RED;   // costante di riduzione
};

array_ptr *array_ptr_init(int length){
    array_ptr *a;
    a = malloc(sizeof(array_ptr));
    a->C_EXP=2;
    a->C_RED=3;
    int size = length * a->C_EXP;
    a->items = malloc(size * sizeof(void *));
    assert (size == 0 || a->items != NULL); 
    a->length = length;
    a->size = size; 
    //printf ("Creazione array: length = %d, size = %d\n", length, size);
    return a;
}


void array_ptr_free(array_ptr *a){
    free(a->items);
    a->items = NULL;
    a->length = a->size = 0;
    free(a);
}

void array_ptr_resize(array_ptr *a, int length){
    if (length > a->size || length < 1.0 * a->size / a->C_RED) {
        int size = length * a->C_EXP;
        a->items = realloc(a->items, size * sizeof(void *));
        assert (size == 0 || a->items != NULL); 
        a->size = size;        
    }
    a->length = length;
    //printf ("Ridimensionamento array: length = %d, size = %d\n", length, a->size);
}

//Adds an element at the end of the array
void array_ptr_push(array_ptr *a, void *elem){
    array_int_resize(a, a->length + 1);
    a->items[a->length - 1] = elem;
}

//Pops last element of the array
void *array_ptr_pop(array_ptr *a){
    void *item = a->items[a->length - 1];
    array_int_resize(a, a->length - 1);
    return item;
}

//Gets element at pos
void *array_ptr_get(array_ptr *a, int pos){
    return a->items[pos];
}

//Gets number of elements in the array
int array_ptr_length(array_ptr *a){
    return a->length;
} 

//Sets element at pos
void array_ptr_set(array_ptr *a, int pos, void *elem){
    if (pos >= a->length)
        array_int_resize(a, pos + 1);
    a->items[pos] = elem;
}

//Prints all the elements of the array
void array_ptr_print(array_ptr *a){
    for (int i = 0; i < a->length; ++i)
        printf ("%p ", a->items[i]);
    printf ("\n");
}
