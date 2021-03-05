#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define NICK_SIZE 6
#define NOME_SIZE 26
#define MAX_SEGUIDOS 100

/* Estrutura do node do user seguido */
typedef struct node_ {
    char nick[NICK_SIZE];   // Nick do ueser seguido
	int ultima_lida;        // Ultima mensagem lida
    int lidas;              // Numero de mensagens lidas
    int pointer;            // Index do user seguido
} node_seg;

/* Estrutura do array de seguidos */
typedef struct info_segue_ {
    int size;                   // Numero de seguidos
	node_seg *segue[MAX_SEGUIDOS];   // Array de seguidos
} info_segue;

void destroy_info(info_segue *info);
info_segue *new_seguidos();
void write_all_seguidos(FILE *fd, info_segue *info);
void write_seguidos(FILE *fd, info_segue *info);
void write_seguidos_D(FILE *fd, info_segue *info);
info_segue *read_all_seguidos(FILE *fd);
info_segue *read_seguidos(FILE *fd);

bool seguidos_empty(info_segue *info);
bool seguidos_full(info_segue *info);
void print_seguidos(info_segue *info);
int inserir_seguido (info_segue *info, char nick[NICK_SIZE], int ultima_lida);
bool remover_seguido (info_segue *info, char nick[NICK_SIZE]);
int numero_seguidos(info_segue *info);