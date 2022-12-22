#ifndef BITARRAY_H
#define BITARRAY_H

#define set_bit(A,k)     ( A[(k)/32] |= (1 << ((k)%32)) )
#define clear_bit(A,k)   ( A[(k)/32] &= ~(1 << ((k)%32)) )
#define test_bit(A,k)    ( A[(k)/32] & (1 << ((k)%32)) )

#endif