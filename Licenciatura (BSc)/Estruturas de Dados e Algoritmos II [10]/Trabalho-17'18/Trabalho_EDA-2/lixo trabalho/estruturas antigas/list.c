#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"


//Feito
typedef struct node_ {
	int ultima_lida;
    int lidas;
    char nick[NICK_SIZE];
	struct node_ *next;
} node;

typedef struct lista_ {
	int size; // N de pessoas que segue
    node *head;
} lista;

//Feito
node *lista_node_new(User *user)
{
	node *n = malloc(sizeof(node));
    strcpy(n->nick, get_nick(user));
    n->ultima_lida = get_mensagens(user);
    n->lidas = 0;

	n->next = NULL;

	return n;
}

//Feito
void node_destroy(node *node)
{
	if (node == NULL)
        return;

    node_destroy(node->next);

    free(node);
}

//Feito
//Feito
lista *list_new()
{
	lista *l = malloc(sizeof(lista));

	l->head = NULL;
	l->size = 0;

	return l;
}

//Feito
void list_destroy(lista *list)
{
    if (!list_empty(list))
        node_destroy(list->head);

    free(list);
}

//Feito         Verrifica se a lista esta vazia
bool list_empty(lista *lista)
{
	return (lista->head == NULL);
}

//Feito         Verrifica se a lista esta cheia
bool list_full(lista *lista)
{
	return lista->size >= MAX_SIZE;
}

//Feito         Inserir na lista
int list_insert(lista *lista, User *user)
{
    if(list_empty(lista))
    {
	    node *n = lista_node_new(user);
	    lista->head = n;

    	lista->size = lista->size + 1;

        return 0;
    }

    node *n = lista_node_new(user);

    char nick2[NICK_SIZE];
    strcpy(nick2, lista->head->nick);     //user de compaação
    node *user_anterior = lista->head;
    node *user_seguinte = user_anterior;
    int comp = strcmp(n->nick, user_anterior->nick);

    if (comp < 0)
    {
        if(list_full(lista)) 
        {
            free(n);
            return 1;
        } // chegou ao n max de seguidores

        
        n->next = user_seguinte;
        lista->head = n;
        lista->size = lista->size + 1;  
        return 0; // foi inserido um novo seguidor
    }
    
    else if (comp == 0) 
    {
        free(n);
        //   printf(    "user seguinte esta REPETIDO\n");  //para testes
        return 2;  // user_1 ja foi inserido
    }

    else
    {
        //   user_2 = user_2->next;
        while (comp > 0 && user_seguinte->next != NULL)
        {
            user_anterior = user_seguinte;
            user_seguinte = user_seguinte->next;
            comp = strcmp(n->nick, user_seguinte->nick);        

            if (comp < 0)
            {
                if(list_full(lista)) 
                {
                    free(n);
                    return 1;
                } // chegou ao n max de seguidores

                n->next = user_seguinte;
                user_anterior->next = n;
                lista->size = lista->size + 1;     
                return 0; // foi inserido um novo seguidor
            }
            
            else if (comp == 0) 
            {
                free(n);
                // printf("user seguinte esta REPETIDO\n");  //para testes
                return 2; // user_1 ja foi inserido
            }
        }

        if (user_seguinte->next == NULL)
        {
            if(list_full(lista)) 
            {
                free(n);
                return 1;
            } // chegou ao n max de seguidores

            user_seguinte->next = n;
            lista->size = lista->size + 1;     
            return 0; // foi inserido um novo seguidor
        }
    }

    return 0;
    
}

//Feito         Remover da lista (deixar de seguir)
bool list_remove(lista *lista, char nick[NICK_SIZE])
{
    if(list_empty(lista))
    {
        return false; // lista vazia
    }

    else
    {
        if(lista->head->next == NULL) //se so segue um user
        {
            int comp = strcmp(nick, lista->head->nick);
            if (comp == 0)
            {
                free(lista->head);// remove head
                lista->size = lista->size - 1;
                //list_destroy(lista);
                return true; // nick removido
            }
        }

        else if(lista->head->next != NULL) //se segue varios users
        {
        node *user_anterior = lista->head;
        node *user_atual = user_anterior->next;
        node *user_seguinte = user_atual->next;
            
            

            int comp = strcmp(nick, lista->head->nick);
            if (comp == 0)
            {
                free(user_anterior);// remove head
                lista->head = user_atual;
                lista->size = lista->size - 1;
                return true; // nick removido
            }

            comp = strcmp(nick, user_atual->nick);
            
            while(comp >= 0)
            {
                if(comp == 0)
                {
                    if(user_seguinte == NULL)
                    {
                        free(user_atual);
                        user_anterior->next = NULL; // remove atual
                        lista->size = lista->size - 1;             
                        return true;
                    }
                    free(user_atual);
                    user_anterior->next = user_seguinte; // remove atual
                    lista->size = lista->size - 1;            
                    return true;
                }

                if (user_seguinte != NULL)
                {
                    user_anterior = user_anterior->next;
                    user_atual = user_anterior->next;
                    if(user_atual->next == NULL)
                    {
                        user_seguinte = NULL;
                    }
                    else
                    {
                        user_seguinte = user_atual->next;
                    }
                    comp = strcmp(nick, user_atual->nick);
                }

                if (user_seguinte == NULL)
                {
                    return false;
                }      
            }
        }
        return false; // nao existe este nick
    }
}


//Feito         ler mensagens
void write_node(node *n, FILE *fd)
{
    fwrite(&n->ultima_lida, sizeof(n->ultima_lida), 1, fd);
    fwrite(&n->lidas, sizeof(n->lidas), 1, fd);

    fwrite(n->nick, NICK_SIZE * sizeof(char), 1, fd);


    if (n->next != NULL)
        write_node(n->next, fd);
}

void write_lista(lista *lista, FILE *fd)
{
    if (lista == NULL)
        return;

    fwrite(&lista->size, sizeof(lista->size), 1, fd);

    if (lista->head != NULL)
        write_node(lista->head, fd);
}

void add_ultima_lida(node *n, int ultima_lida)
{
    n->ultima_lida = ultima_lida;
}

void set_head(lista *lista, node *n)
{
    lista->head = n;
}

void add_lidas(node *n, int lidas)
{
    if (n != NULL && lidas >= 0)
    n->lidas = lidas;
}

void set_next_node(node *n, node *next)
{
    n->next = next;
}


lista *read_size(lista *lista, FILE *fd)
{
    lista = list_new();
    
    fread(&lista->size, sizeof(int), 1, fd);

    return lista;
}

//Feito
int get_size(lista *lista)
{
    return lista->size;
}

//Feito
void print_lista_segue(lista *lista){

    if (list_empty(lista))
    {
     //   printf("Ta vazio!\n");
        return;
    }

    node *seguinte = lista->head;
    char *nick;
        
    while (seguinte != NULL)
        {
            nick = seguinte->nick;
          /*  if (seguinte->ultima_lida == mensagens)    
                printf("%s (%d lidas)\n", nick, mensagens);
            else*/
                printf("%s (%d lidas)\n", nick, seguinte->ultima_lida);
            seguinte = seguinte->next;         
        }
}

node *get_head(lista * l)
{
    return l->head;
}

char *get_list_nick(node *n)
{
    return n->nick;
}

int get_ultima_lida(node *n)
{
    return n->ultima_lida;
}

int get_lidas(node *n)
{
    if (n != NULL)
        return n->lidas;

    return -1;
}

node *get_next_node(node *n)
{
    return n->next;
}