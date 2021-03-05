#include <stdio.h>
#include <stdbool.h>
#include "user.h"

#define MAX_USERS 2500009   // (um milhão + 10%) * 2 -> primo a seguir
#define MAX_PRIME 2499997   // primo inferior e mais próximo de MAX_USERS   
#define FICH_HT "hashtable.dat"

typedef struct hashtable Hashtable;

Hashtable *read_hashtable();
FILE *open_hashfile();

void insert_item(FILE *fd, Hashtable *ht, int ptr, char nick[NICK_SIZE]);
int search_ptr(Hashtable *ht, char nick[NICK_SIZE]);

void destroy_hashtable(Hashtable *ht);