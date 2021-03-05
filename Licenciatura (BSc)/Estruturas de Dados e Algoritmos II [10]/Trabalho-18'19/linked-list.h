#define _LIST_H 1

#ifndef _AEROPORTOS_H
#include "aeroportos.h"
#endif

#include <stdbool.h>
#include <stdio.h>

#define MAX_LENGTH 150

// lista de adjacências.

typedef struct node {
    char cod_voo[MAX_COD_VOO];
    struct node *next;
} Node;

typedef struct list {
    Node *head;
    short length;   // no maximo 150
} List;

Node *node_new(char cod_voo[MAX_COD_VOO]);
List *list_new();
void list_destroy(List *list);
bool list_insert(List *list, char cod_voo[MAX_COD_VOO]);
void list_remove(List *list, char cod_voo[MAX_COD_VOO]);
short list_length(List *list);
void list_write(List *list, FILE *fd);
List *list_read(FILE *fd);
void list_print(List *list);