#include <stdio.h>

#include "user.h"
#include "list.h"
#include "seguidores.h"

#define MAX_SIZE 2200031
#define NOME_FICH "hashtable.bin"

typedef struct cell {
    User *user;
    lista *segue;
    Seguidores *seguidores;
} Cell;

Cell *new_cell()
{
    Cell *c = malloc(sizeof(Cell));

    c->user = NULL;
    c->segue = NULL;
    c->seguidores = NULL;

    return c;
}

unsigned long hash(unsigned char *str)
    {
        unsigned long hash = 5381;
        int c;

        while (c = *str++)
            hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

        return hash % MAX_SIZE;
    }


FILE *open_fich()
{
    FILE *fd = fopen(NOME_FICH, "r+");

    if (fd == NULL)
        fd = fopen(NOME_FICH, "w+");

    return fd;
}

void write_cell(Cell *cwrite, User *u, FILE *fd)
{
    rewind(fd);
    long hash = string_hash(get_nick(u));
    fseek(fd, hash * sizeof(Cell), SEEK_SET);

    Cell *c;

    fread(c, sizeof(Cell), 1, fd);
    while (c->user != NULL)
        fread(c, sizeof(Cell), 1, fd);

    fseek(fd, -sizeof(Cell), SEEK_CUR);
    fwrite(cwrite, sizeof(Cell), 1, fd);
}

Cell *read_cell(char nick[NICK_SIZE],  FILE *fd)
{
    rewind(fd);
    long hash = string_hash(nick);
    fseek(fd, hash * sizeof(Cell), SEEK_SET);

    Cell *c;

    fread(c, sizeof(Cell), 1, fd);
    if (c == NULL)
        return NULL;

    while (strcmp(get_nick(c->user), nick) != 0)
        fread(c, sizeof(Cell), 1, fd);
    
    return c;
}

bool insert_user(User *u, FILE *fd)
{
    Cell *c = new_cell();
    c->user = u;
    c->segue = list_new();
    c->seguidores = new_seguidores();

    if (read_cell(get_nick(u), fd)->user != NULL)
        return false;

    write_cell(c, u, fd);
}

User *get_user(char nick[NICK_SIZE], FILE *fd)
{
    Cell *c = read_cell(nick, fd);

    return c->user;
}

lista *find_segue(char nick[NICK_SIZE], FILE *fd)
{
    read_cell;
}

