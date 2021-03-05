#include <stdbool.h>
#include "user.h"

#define MAX_SIZE 100;

typedef struct lista_ lista;
typedef struct node_ node;

node *lista_node_new(User *user);

lista *list_new();
void list_destroy(lista *list);

bool list_empty(lista *lista);
bool list_full(lista *lista);
int list_insert(lista *lista, User *user);
bool list_remove(lista *lista, char nick[NICK_SIZE]);

void diminuir_size(lista *lista);

void add_ultima_lida(node *n, int ultima_lida);

int get_ultima_lida(node *n);

char *get_list_nick(node *n);

void add_lidas(node *n, int lidas);
int get_lidas(node *n);
void set_next_node(node *n, node *next);
node *get_next_node(node *n);
void set_head(lista *lista, node *n);
node *get_head(lista * l);

void write_lista(lista *lista, FILE *fd);
lista *read_size(lista *lista, FILE *fd);

int get_size(lista *lista);
void print_lista_segue(lista *lista);

