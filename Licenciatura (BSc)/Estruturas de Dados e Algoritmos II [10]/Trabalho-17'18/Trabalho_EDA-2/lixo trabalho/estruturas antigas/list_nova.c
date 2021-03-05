#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

//Feito - nos da lista
typedef struct node_ {
	int ultima_lida;
    int lidas;
    char nick[NICK_SIZE];
	struct node_ *next;
} node;

node *lista_node_new(User *user)
{
    node *n = malloc(sizeof(node));
    strcpy(n->nick, get_nick(user));
    n->ultima_lida = get_mensagens(user);
    n->lidas = 0;
    n->next = NULL;
    return n;
}

void lista_node_destroy(node *node)
{
    if (node == NULL)
        return;

    lista_node_destroy(node->next);
    free(node);
}

//////////////////////////////////////////////////
//Feito - lista
typedef struct lista_ {
	int size; // N de pessoas que segue
    node *head;
} lista;

lista *lista_new()
{
    lista *l = malloc(sizeof(lista));
    l->head = NULL;
    l->size = 0;
    return l;
}

void lista_destroy(lista *list)
{
    if (!list_empty(list))
        lista_node_destroy(list->head);
    
    free(list);
}

// Verrifica se a lista esta vazia
bool list_empty(lista *lista)
{
	return (lista->head == NULL);
}

// Verrifica se a lista esta cheia
bool list_full(lista *lista)
{
	return lista->size >= MAX_SIZE;
}

// retorna o numero de users que segue (seguidos)
int get_lista_size(lista *lista)
{
    return lista->size;
}



//////////////////////////////////////////////////////////

// listar todos os seguidos
void print_lista_seguidos(lista* lista)
{
    if (list_empty(lista))
        return;

    node *seguinte = lista->head;
    char *nick[NICK_SIZE];

    while (seguinte != NULL)
    {
        nick[NICK_SIZE] = seguinte->nick;
        printf("%s (%d lidas)\n", nick, seguinte->ultima_lida);
        seguinte = seguinte->next;
    }
}

// inserir na lista
int list_insert(lista *lista, User *user)
{
    node *n = lista_node_new(user);
    char new_nick[NICK_SIZE]; // nick que vai ser inserido
    strcpy(new_nick, n->nick);

    if (list_empty(lista))
    {
        lista->head = n;
        lista->size = lista->size + 1;
        return 0; //foi inserido com sucesso 
    }

    node *user_anterior = lista->head;
    node *user_seguinte = user_anterior->next;
    char nick2[NICK_SIZE]; // nick que esta na lista (usado para comprarar)
    strcpy(nick2, user_anterior->nick);

    int comp = strcmp(new_nick, nick2);

    if (comp == 0)
    {
        free(n);
        return 2; // ja foi inserido anteriormente
    }

    if (comp < 0) // mete o novo nick na primeira posicao da lista
    {
        if(list_full(lista)) 
        {
            free(n);
            return 1; // chegou ao n max de seguidores
        }
        n->next = user_anterior;
        lista->head = n;
        lista->size = lista->size + 1;
        return 0; //foi inserido com sucesso 
    }

    while (comp > 0 && user_seguinte != NULL)
    {
        strcpy(nick2, user_seguinte->nick);
        comp = strcmp(new_nick, nick2);

        if (comp == 0) 
        {
            free(n);
            return 2; // ja foi inserido anteriormente
        }

        if (comp < 0)
        {
            if(list_full(lista)) 
            {
                free(n);
                return 1; // chegou ao n max de seguidores
            }
            user_anterior->next = n;
            n->next = user_seguinte;
            lista->size = lista->size + 1;
            return 0; //foi inserido com sucesso
        }

        if (user_seguinte->next == NULL) // inserir na ultima posicao
        {
            if(list_full(lista)) 
            {
                free(n);
                return 1; // chegou ao n max de seguidores
            }
            user_seguinte->next = n;
            lista->size = lista->size + 1;            
            return 0; //foi inserido com sucesso
        }
    user_anterior = user_anterior->next;
    user_seguinte = user_seguinte->next;
    }

    free(n);
    printf("alguma coisa esta mal\n");
    return 1;

}


// remover da lista (deixar de seguir)
bool list_remove(lista *lista, char nick[NICK_SIZE])
{
    if(list_empty(lista))
    {
        return false; // lista vazia (nenhum nick foi removido)
    }
    
    char nick2[NICK_SIZE]; // nick que esta na lista (usado para comprarar)
    strcpy(nick2, lista->head->nick);

    int comp = strcmp(nick, nick2);

    if (lista->head->next == NULL) //segue apenas um user
    {
        if (comp == 0)
        {
            free(lista->head); // remove o unico seguido
            lista->size = lista->size - 1;
            return true; // nick removido
        }
    }

    node *user_anterior = lista->head;
    node *user_atual = user_anterior->next; // user usado na comparacao
    node *user_seguinte = user_atual->next;

// se seguir varios user faz o seguinte:
    strcpy(nick2, user_anterior->nick);
    comp = strcmp(nick, nick2);
    if (comp == 0)
    {
        free(user_anterior); // remove o primeiro da lista
        lista->head = user_atual;
        lista->size = lista->size - 1;
        return true; // nick removido
    }

    while (comp >= 0 && user_atual != NULL)
    {
        strcpy(nick2, user_atual->nick);
        comp = strcmp(nick, nick2);

        if (comp == 0)
        {
            free(user_atual);
            user_anterior->next = user_seguinte;
            lista->size = lista->size - 1;
            return true; // nick removido
        }

        node *user_anterior = lista->head;
        node *user_atual = user_anterior->next; // user usado na comparacao
        node *user_seguinte = user_atual->next;
    }

    return false; // nenhum nick foi removido (o nick nao existe na lista)
}