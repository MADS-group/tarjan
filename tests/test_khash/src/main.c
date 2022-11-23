#include <stdlib.h>
#include <stdio.h>
#include "khash.h"

KHASH_MAP_INIT_INT(m32, int)
int main(int argc, char* argv[]){
    (void) argc;
    (void) argv;
    khash_t(m32) *ht;
    khint_t k;
    int ret;
    ht = kh_init(m32);

    for (int i = 1; i <= 100; ++i) {
		k = kh_put(m32, ht, i, &ret);
		kh_val(ht, k) = k;
	}
    for (int i = 1; i <= 200; ++i) {
		k = kh_put(m32, ht, i, &ret);
        if(ret!=0){ //If key is not present in the ht put smth
            kh_value(ht, k) = 42;
        }
        printf("k: %d ret:%d value:%d exists:%d\n", k , ret, kh_value(ht, k), kh_exist(ht, k));
	}
    kh_put(m32, ht, 7777, &ret);
    for (int i = 1; i <= 400; ++i) {
        printf("k: %d exists:%d\n", i,kh_exist(ht,i));
	}

    k = kh_get(m32, ht, 0);
    printf("k: %d, exist: %d, val: %d, kh_end: %d\n",k,kh_exist(ht, k),kh_value(ht,k), kh_end(ht));
    
    kh_destroy(m32, ht);
}