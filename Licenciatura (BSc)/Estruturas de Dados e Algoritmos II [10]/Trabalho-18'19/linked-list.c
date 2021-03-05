#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "linked-list.h"


/* Contrutor de um no da lista */
Node *node_new(char cod_voo[MAX_COD_VOO])
{
    Node *node = malloc(sizeof(*node));
    
    strncpy(node->cod_voo, cod_voo, MAX_COD_VOO);
    node->next = NULL;

    return node;
}

/* Construtor de uma lista */
List *list_new()
{
    List *list = malloc(sizeof(*list));

    list->head = node_new(""); // no dummy
    list->length = 0;

    return list;
}

/* libertar memoria de nos de uma lista, recursivamente */
void node_destroy(Node *node)
{
    if (node->next != NULL)
        node_destroy(node->next); 
    
    free(node);
}

/* libertar memoria de uma lista */
void list_destroy(List *list) 
{  
    if (list->head != NULL)
        node_destroy(list->head);
    free(list);
}

/* Inserir um no com um voo na lista.
   A lista encontra-se ordenada.
   Primeiro verifica-se se o voo já se encontra na lista.
   Depois insere-se no sitio certo.                        */
bool list_insert(List *list, char cod_voo[MAX_COD_VOO])
{
    Node *novo = node_new(cod_voo);
    Node *atual = list->head;
    
    while (atual->next != NULL && strncmp(atual->next->cod_voo, novo->cod_voo, MAX_COD_VOO) < 0)
        atual = atual->next;

    // caso novo voo seja o "maior"
    if (atual->next == NULL)
    {
        atual->next = novo;
        list->length += 1;
        return true;
    }

    // caso novo voo devesse existir no meio da lista
    if (strncmp(atual->next->cod_voo, novo->cod_voo, MAX_COD_VOO) > 0)
    {
        Node *temp = atual->next;
        atual->next = novo;
        novo->next = temp;  
        list->length += 1;
        return true;
    }

    // caso novo voo ja exista
    else
    {
        free(novo);
        return false;
    }
}

/* Remover voo da lista. Assume-se que voo existe. */
void list_remove(List *list, char cod_voo[MAX_COD_VOO]) 
{
    Node *atual = list->head;

    while (atual->next != NULL && strncmp(atual->next->cod_voo, cod_voo, MAX_COD_VOO) < 0)////////////////
        atual = atual->next;

    if (atual->next != NULL && strncmp(atual->next->cod_voo, cod_voo, MAX_COD_VOO) == 0)/////////////////////////////////
    {
        Node *a_remover = atual->next;
        atual->next = atual->next->next;
        free(a_remover);
        list->length -= 1;
    }
    else
    {
        printf("ERRO! Voo a remover nao existe, verifique a sua existencia antes de remover na lista.");
    }
}

/* Ocupacao da lista */
short list_length(List *list)
{
    return list->length;
}

void node_write(Node *node, FILE *fd)
{
    fwrite(node->cod_voo, sizeof(char) * MAX_COD_VOO, 1, fd);
    if (node->next != NULL)
        node_write(node->next, fd);
}

void list_write(List *list, FILE *fd)
{
    fwrite(&list->length, sizeof(short), 1, fd);
    Node *atual = list->head->next;    
    for (int i = 0; i < list->length; i++)
    {
        fwrite(atual->cod_voo, sizeof(char)*MAX_COD_VOO, 1, fd);
        atual = atual->next;
    }
    
    char vazio[MAX_COD_VOO] = "      ";
    for (int i = list->length; i < MAX_LENGTH; i++)
        fwrite(vazio,sizeof(char)*MAX_COD_VOO, 1, fd);
}

List *list_read(FILE *fd)
{
    List *list = list_new();
    fread(&list->length, sizeof(short), 1, fd);
    strncpy(list->head->cod_voo, "", MAX_COD_VOO);    // no dummy
    
    char cod_voo[MAX_COD_VOO];
    Node *ler = list->head;
    for (int i = 0; i < list->length; i++)
    {
        fread(cod_voo, sizeof(char) * MAX_COD_VOO, 1, fd);
        ler->next = node_new(cod_voo);
        ler = ler->next;
    }

    return list;
}

void list_print(List *list)
{
    Node *atual = list->head->next;
    printf("Lista: ");
    while (atual != NULL)
    {
        printf("%s |", atual->cod_voo);
        atual = atual->next;
    }
    printf("\n");
}