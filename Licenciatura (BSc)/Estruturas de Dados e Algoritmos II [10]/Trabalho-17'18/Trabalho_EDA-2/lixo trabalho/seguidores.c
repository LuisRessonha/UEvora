#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "seguidores.h"
#include "user.h"

typedef struct nodeSeg {
    char nick[NICK_SIZE];
    struct nodeSeg *next;
} NodeSeg;

NodeSeg *new_nodeSeg(char nick[NICK_SIZE])
{
    NodeSeg *n = malloc(sizeof(NodeSeg));
    
    strcpy(n->nick, nick);
    n->next = NULL;

    return n;
}

void destroy_node(NodeSeg *n)
{
    free(n);
}

typedef struct seguidores {
    int size;
    NodeSeg *head;
} Seguidores;

Seguidores *new_seguidores()
{
    Seguidores *s = malloc(sizeof(Seguidores));

    s->head = NULL;
    s->size = 0;

    return s;
}

void destroy_seguidores(Seguidores *s)
{
    NodeSeg *n_atual = s->head;
    NodeSeg *n_seguinte;

    while (n_atual != NULL)
    {
        n_seguinte = n_atual->next;
        destroy_node(n_atual);
        n_atual = n_seguinte;
    }
}

void add_seguidor(Seguidores *seg, char nick[NICK_SIZE])
{
    NodeSeg *n = new_nodeSeg(nick);

    if (seg->head == NULL)
        seg->head = n;
    else
    {
        NodeSeg *next = seg->head;
        seg->head = n;
        seg->head->next = next;
    }

    seg->size++;
}

void remove_seguidor(Seguidores *seg, char nick[NICK_SIZE])
{
    NodeSeg *atual = seg->head;
    if (atual != NULL && strcmp(atual->nick, nick) == 0)
    {
        NodeSeg *next = atual->next;
        destroy_node(atual);
        seg->head = next;
        seg->size = seg->size - 1;
        
        return;
    }

    while (atual->next != NULL && strcmp(atual->next->nick, nick) != 0)
        atual = atual->next;

    if (strcmp(atual->next->nick, nick) == 0)
    {
        NodeSeg *rem = atual->next;
        atual->next = rem->next;

        seg->size = seg->size - 1;
        destroy_node(rem);
        return;
    }
}

void desativar_seguidores(Seguidores *seg, Trie *trie)
{
    for (NodeSeg *n_atual = seg->head; n_atual != NULL; n_atual = n_atual->next)
        diminuir_size(trie_find_segue(trie, n_atual->nick));
}

int numero_seguidores(Seguidores *seg)
{
    return seg->size;
}

void write_nodeseg(NodeSeg *n, FILE *fd)
{
    fwrite(n->nick, NICK_SIZE * sizeof(char), 1, fd);

    if (n->next != NULL)
        write_nodeseg(n->next, fd);
}

void write_seguidores(Seguidores *seg, FILE *fd)
{  
    fwrite(&seg->size, sizeof(seg->size), 1, fd);

    if (seg->head != NULL)
        write_nodeseg(seg->head, fd);
}

NodeSeg *read_node(FILE *fd, int counter)
{
    char nick[NICK_SIZE];
    NodeSeg *n;

    fread(nick, NICK_SIZE * sizeof(char), 1, fd);

    n = new_nodeSeg(nick);
    counter -= 1;
    if (counter != 0)
        n->next = read_node(fd, counter);

    return n;
}

Seguidores  *read_seguidores(Seguidores *seg, FILE *fd)
{
    if (seg->size != 0)
        seg->head = read_node(fd, seg->size);

    return seg;
}

Seguidores *read_segSize(Seguidores *seg, FILE *fd)
{
    seg = new_seguidores();

    fread(&seg->size, sizeof(int), 1, fd);

    return seg;
}
