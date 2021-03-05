#define _HTAP_H 1

#ifndef _LIST_H
#include "linked-list.h"
#endif

#include <stdbool.h>
#include <stdio.h>

/* Primo mais perto de 400000, que é a capacidade da hashtable
   de modo a que o fator carga seja 50%.         */
#define MAX_AEROPORTOS_HASH 400009

typedef struct cell {
    bool modificado;
    Aeroporto *ap;
    List *voos;
} Cell;

typedef struct hashtable {
    int n_ap;
    Cell *table[MAX_AEROPORTOS_HASH]; 
} Ht_ap;

void cell_destroy(Cell *cell);
void cell_write(Cell *cell, FILE *fd);
FILE *open_file_ap();
Cell *cell_read(FILE *fd, char cod_ap[MAX_COD_AP]);
Ht_ap *hashtable_ap_new(FILE *fd_ap);
void hashtable_ap_write_and_destroy(Ht_ap *hash, FILE *fd);
bool hashtable_ap_insert(Ht_ap *hash, FILE *fd_ap, Aeroporto *ap);
Cell *hashtable_ap_get(Ht_ap *hash, FILE *fd, char cod_ap[MAX_COD_AP]);