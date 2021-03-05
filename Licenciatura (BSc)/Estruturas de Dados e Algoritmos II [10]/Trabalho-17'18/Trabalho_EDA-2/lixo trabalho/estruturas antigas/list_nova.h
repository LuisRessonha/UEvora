#include <stdbool.h>
#include "user.h"

#define MAX_SIZE 100;

typedef struct lista_ lista;
typedef struct node_ node;

node *lista_node_new(User *user);
void lista_node_destroy(node *node);
lista *lista_new();
void lista_destroy(lista *list);
bool list_empty(lista *lista);
bool list_full(lista *lista);
int get_lista_size(lista *lista);
void print_lista_seguidos(lista* lista);
int list_insert(lista *lista, User *user);
bool list_remove(lista *lista, char nick[NICK_SIZE]);
