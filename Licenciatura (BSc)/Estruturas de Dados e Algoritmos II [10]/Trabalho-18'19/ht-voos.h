#define _HTVOO_H 1

#ifndef _HTAP_H
#include "ht-aeroportos.h"
#endif

#ifndef _AEROPORTOS_H
#include "aeroportos.h"
#endif

#include <stdbool.h>
#include <stdio.h>

#define MAX_VOOS 1500007    

typedef struct hashtable_voo {
    Voo *table[MAX_VOOS];
} Ht_voo;

FILE *open_file_voo();
Ht_voo *hashtable_voo_new();
void hashtable_voo_write_and_destroy(Ht_voo *hash, FILE *fd);
void hashtable_voo_read(Ht_voo *hash, FILE *fd);
bool hashtable_voo_insert(Ht_voo *hash, FILE *fd, Voo *voo);
Voo *hashtable_voo_get(Ht_voo *hash, FILE *fd_voo, char cod_voo[MAX_COD_VOO]);
bool hashtable_voo_remove(Ht_voo *hash, Ht_ap *hash_ap, FILE *fd_voo, FILE *fd_ap, char cod_voo[MAX_COD_VOO]);

