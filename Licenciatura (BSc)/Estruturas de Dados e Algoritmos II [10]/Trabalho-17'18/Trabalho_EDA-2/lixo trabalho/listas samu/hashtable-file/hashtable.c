#include <stdio.h>
#include <stdlib.h>
#include "user.h"

#define NOME_FICHEIRO "hashtable.bin"
#define MAX 1100009

typedef struct quarto {
    char nick[NICK_SIZE];
    char edificio;
    int piso;
    int numero;   
} Quarto;

Quarto *new_quarto()
{
    Quarto *q = malloc(sizeof(Quarto));

    q->nick = "pau";
    q->edificio = ' ';
    q->numero = 0;
    q->piso = 0;

    return q;
}

unsigned int string_hash(void *string)
{
	/* This is the djb2 string hash function */

	unsigned int result = MAX;
	unsigned char *p;

	p = (unsigned char *) string;

	while (*p != '\0') {
		result = (result << 5) + result + *p;
		++p;
	}

	return result;
}

FILE *open_fich()
{
    FILE *fd = fopen(NOME_FICHEIRO, "r+");

    if (fd == NULL)
        fd = fopen(NOME_FICHEIRO, "w+");

    return fd;
}

Quarto *find(char nick[NICK_SIZE] , FILE *fd)
{
    int hash = string_hash(q->nick);

    fseek(fd, sizeof(q) * hash, SEEK_SET);

    Quarto *q;
    fread(q, sizeof(q), 1, fd);

    return q;
}

bool insert(Quarto *q)
{
    FILE *fd = open_fich();
    
    Quarto *q_read = find(q->nick, fd);

    while (strcmp(q_read->nick, q->nick) != 0)
    {
        fseek(fd, sizeof(q), SEEK_CUR);
        fread(q_read, sizeof(q), 1, fd);
    }


   
}
