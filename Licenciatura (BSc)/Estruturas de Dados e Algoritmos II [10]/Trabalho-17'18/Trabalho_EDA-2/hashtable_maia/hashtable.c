//
// Created by Daniel on 10/06/2018.
//

#include <memory.h>
#include "hashtable.h"

const int max = 2500009;
int size = 0;
int prime = 2499997;

struct hashtable_item *array;

int hashcode1(unsigned long long key)
{
    return (key % max);
}

int hashcode2(unsigned long long key)
{
    return (prime - (key % prime));
}

unsigned long long ht_calc_hash (char* key) {
    unsigned long long h = 5381;
    while (*(key++)){
        h = ((h << 5) + h) + (*key);
    }
    return h;
}

int find(unsigned long long key, char nick[6]) {
    int hash1 = hashcode1(key);
    int hash2 = hashcode2(key);

    int index = hash1;

    while (array[index].flag !=0) {
        if (strcmp(array[index].item.nick, nick) == 0) {
            if (array[index].flag==1) {
                return index;
            }
            else {
                index = -1;
                return index;
            }
        }
        index = (index + hash2) % max;

    }
    index=-1;
    return index;
}

