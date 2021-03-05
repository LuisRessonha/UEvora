#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "seguidos.h"

#define NICK_SIZE 6
#define NOME_SIZE 26
#define FICH_USERS "users.dat"

typedef struct user {
    char nick[NICK_SIZE];
    char nome[NOME_SIZE];
    bool apagado;
    int mensagens;
    int seguidores;
    info_segue *info;
} User;

void destroy_user(User *u);

User *read_user(FILE *fd, int index);
void write_all_user(FILE *fd, User *u, int index);
void write_user(FILE *fd, User *u, int index);
void write_user_D(FILE *fd, User *u, int index);

int new_user(FILE *fd, char nick[NICK_SIZE], char nome[NOME_SIZE]);
bool apagar_user(FILE *fd, int index);
int get_mensagens(FILE *fd, int index);
char *get_nome(FILE *fd, int index);

bool apagado(FILE *fd, int index);
void adicionar_seguidor(FILE *fd, int index);
void remover_seguidor(FILE *fd, int index);
void adicionar_mensagem(User *u);
void ler_mensagem(info_segue *info, FILE *fd);

//void print_user(User *u);