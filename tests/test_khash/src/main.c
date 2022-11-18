#include <stdlib.h>
#include <stdio.h>
#include "khash.h"

KHASH_MAP_INIT_INT(m32, int)
int main(int argc, char* argv[]){
    khash_t(m32) *ht;
    khint_t k;
    int ret;
    ht = kh_init(m32);

    for (int i = 1; i <= 10000; ++i) {
		k = kh_put(m32, ht, i, &ret);
		kh_val(ht, k) = k;
	}
    int key,value;
    kh_foreach(ht, key, value, {
        printf("key: %d value:%d\n",key,value);
    });
    kh_destroy(m32, ht);
}