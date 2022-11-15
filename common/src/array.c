//Dynamic array implementation
#include "array.h"
#include <stdlib.h>

#define C_EXP 5  // costante di espansione
#define C_RED 10 // costante di riduzione

struct array {
    int *items; // puntatore agli elementi dell'array
    int length;   // lunghezza array    
    int size;     // dimensione allocata (>= length)
};

array arrayCreate (int length) {
    array a;
    int size = length + C_EXP;
    a.items = malloc(size * sizeof(int));
    assert (size == 0 || a.items != NULL); 
    a.length = length;
    a.size = size; 
    printf ("Creazione array: length = %d, size = %d\n", length, size);
    return a;
}

void arrayDestroy (array *a) {
    free(a->items);
    a->items = NULL;
    a->length = a->size = 0;
    printf ("Distruzione array\n");
}

void arrayResize (array *a, int length) {
    if (length > a->size || length < a->size - C_RED) {
        int size = length + C_EXP;
        a->items = realloc(a->items, size * sizeof(int));
        assert (size == 0 || a->items != NULL); 
        a->size = size;        
    }
    a->length = length;
    printf ("Ridimensionamento array: length = %d, size = %d\n", length, a->size);
 }

/* FUNZIONI DI UTILITA' */

// Aggiunta di un elemento in coda all'array
void arrayAdd (array *a, int elem) {
    arrayResize(a, a->length + 1);
    a->items[a->length - 1] = elem;
}

// Lettura di un elemento in posizione pos
int arrayGet (array *a, int pos) {
    return a->items[pos];
}

// Lettura della lunghezza dell'array
int arrayLength (array *a) {
    return a->length;
}

// Inserimento di un elemento in posizione pos con ridimensionamento automatico
void arraySet (array *a, int pos, int elem) {
    if (pos >= a->length)
        arrayResize(a, pos + 1);
    a->items[pos] = elem;
}

// Stampa dell'array
void arrayPrint(array *a) {
    for (int i = 0; i < a->length; ++i)
        printf ("%d ", a->items[i]);
    printf ("\n");
}