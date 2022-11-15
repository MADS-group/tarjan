#ifndef ARRAY_H
#define ARRAY_H

typedef struct array array;

array arrayCreate (int length, int const_exp, int const_red);
void arrayDestroy (array *a);
void arrayResize (array *a, int length);

/* FUNZIONI DI UTILITA' */
void arrayAdd (array *a, int elem);
int arrayGet (array *a, int pos);
int arrayLength (array *a);
void arraySet (array *a, int pos, int elem);
void arrayPrint(array *a);

#endif