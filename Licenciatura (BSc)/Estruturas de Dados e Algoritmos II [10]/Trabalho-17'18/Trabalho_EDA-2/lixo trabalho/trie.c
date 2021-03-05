#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "trie.h"

#define ALPHABET_MIN '0'
#define ALPHABET_MAX 'z'
#define ALPHABET_SIZE (ALPHABET_MAX - ALPHABET_MIN + 1)
#define FICH_USERS "users.dat"
#define FICH_LISTA "listas.dat"

#define POS(c)  ((c) - ALPHABET_MIN)
#define CHAR(n) ((n) + ALPHABET_MIN)

typedef struct node {
    struct node *child[ALPHABET_SIZE];
    char estado;    // '0'->nao existe, '1'->existe, 'x'->apagado
    User *user;
    lista *segue;
} Node;

typedef struct trie {
    Node *root;
} Trie;

Node *trie_node_new()
{
    Node *node = malloc(sizeof(*node));

    node->estado = '0';
    for (int i = 0; i < ALPHABET_SIZE; i++)
        node->child[i] = NULL;

    node->user = NULL;
    node->segue = NULL;

    return node;
}

void node_free(Node *node)
{   
    free(node);
}

void trie_node_destroy(Node *node)
{
    int i;
    if (node == NULL)
        return;

    if (node->user != NULL)
    {
        destroy_user(node->user);
        list_destroy(node->segue);
    }
    for (i = 0; i < ALPHABET_SIZE; i++)
        trie_node_destroy(node->child[i]);

    node_free(node);
}

Trie *trie_new()
{
    Trie *trie = malloc(sizeof(Trie));

    trie->root = NULL;

    return trie;
}

void trie_destroy(Trie *trie)
{
    trie_node_destroy(trie->root);
    free(trie);
}

bool trie_empty(Trie *trie)
{
    return trie->root == NULL;
}

bool trie_insert(Trie *trie, User *user)
{
    Node *n;
    char p[NICK_SIZE];
    strcpy(p, get_nick(user));
    int i = 0;

    if (trie_empty(trie))
        trie->root = trie_node_new();
    
    n = trie->root;

    while (p[i] != '\0' && n->child[POS(p[i])] != NULL)
    {
        n = n->child[POS(p[i])];
        i++;
    }

    while (p[i] != '\0')
    {
        n->child[POS(p[i])] = trie_node_new();
        n = n->child[POS(p[i])];

        i++;
    }


    if (n->estado == '1' || n->estado == 'X')
        return false;
    else
    {
       // print_user(user);
        if (apagado(user))
        {
            n->estado = 'X';
            
        }
        else
        {
            n->estado = '1';
        }
        n->user = user;
        n->segue = list_new();
        return true;   // inserido com sucesso
    }
}

User *trie_find(Trie *trie, char nick[NICK_SIZE])
{
    Node *n;
    int i = 0;

    n = trie->root;

    while (n != NULL && nick[i] != '\0')
    {
        n = n->child[POS(nick[i])];
        i++;
    }

    if (n != NULL && n->estado != '0')
        return n->user;
    
    return NULL;
}

Node *get_node(Trie *trie, char nick[NICK_SIZE])
{
    Node *n;
    int i = 0;

    n = trie->root;

    while (n != NULL && nick[i] != '\0')
    {
        n = n->child[POS(nick[i])];
        i++;
    }

    if (n != NULL && n->estado != '0')
        return n;
    
    return NULL;
}

lista *trie_find_segue(Trie *trie, char nick[NICK_SIZE])
{
    Node *n;
    int i = 0;

    n = trie->root;

    while (n != NULL && nick[i] != '\0')
    {
        n = n->child[POS(nick[i])];
        i++;
    }

    if (n != NULL && n->estado != '0')
        return n->segue;

    return NULL;
}


bool trie_apagar(Trie *trie, char nick[NICK_SIZE])
{
    Node *n;
    int i = 0;

    n = trie->root;

    while (n != NULL && nick[i] != '\0')
    {
        n = n->child[POS(nick[i])];
        i++;
    }

    if (n != NULL && n->estado == '1')
    {
        apagar_user(n->user);
        n->estado = 'X';
        return true;
    }
    else
        return false;
}

void node_write(Node *n, FILE *fd)
{
    if (n == NULL)
        return;

    if (n->estado != '0')
    {
        write_user(n->user, fd);
      //  print_user(n->user);
       // write_lista(n->segue, fd);
       // write_seguidores(n->seguidores, fd);
        
    }
    for (int i = 0; i < ALPHABET_SIZE; i++)
        node_write(n->child[i], fd);
}

void lista_write(Node *n, FILE *fd)
{
    if (n == NULL)
        return;
    
    if (n->estado != '0')
    {
        fwrite(get_nick(n->user), NICK_SIZE * sizeof(char), 1, fd);
        write_lista(n->segue, fd);
    }

    for (int i = 0; i < ALPHABET_SIZE; i++)
        lista_write(n->child[i], fd);

}

void trie_write(Trie *trie)
{
    FILE *fd = fopen(FICH_USERS, "w");    
    node_write(trie->root, fd);
    fclose(fd);

    fd = fopen(FICH_LISTA, "w");
    lista_write(trie->root, fd);
    fclose(fd);

}

node *node_lista(Trie *trie, FILE *fd, int counter)
{
    char nick[NICK_SIZE];
    int ultima_lida, lidas;
    node *n;
    
    fread(&ultima_lida, sizeof(ultima_lida), 1, fd);
    fread(&lidas, sizeof(lidas), 1, fd);
    fread(nick, NICK_SIZE * sizeof(char), 1, fd);

    User *u = trie_find(trie, nick);
    if (u == NULL)
        printf("what. %s\n", nick);

    n = lista_node_new(u);

    add_ultima_lida(n, ultima_lida);
    add_lidas(n, lidas);
    
    counter -= 1;
    if (counter != 0)
        set_next_node(n, node_lista(trie, fd, counter));

    return n;
}

lista *lista_read(Trie *trie, lista *lista, FILE *fd)
{
    if (get_size(lista) != 0)
        set_head(lista, node_lista(trie, fd, get_size(lista)));

    return lista;
}

Trie *trie_read()
{
    FILE *fd = fopen(FICH_USERS, "r");
    Trie *trie = trie_new();

    if (fd == NULL)
        return trie;

    while (true)
    {
        User *u = new_empty_user();

        if (read_user(u, fd) != 1)
        {
            destroy_user(u);
            break;
        }

        trie_insert(trie, u);
      //  Node *n = get_node(trie, get_nick(u));
      //  n->segue = read_lista(n->segue, fd);
       // read_seguidores(n->seguidores, fd);

    }
    fclose(fd);

    fd = fopen(FICH_LISTA, "r");
    
    char nick[NICK_SIZE];
    
    while(fread(nick, sizeof(nick), 1, fd) == 1)
    {
        Node *n = get_node(trie, nick);
        n->segue = read_size(n->segue, fd);
        n->segue = lista_read(trie, n->segue, fd);
    } 

    fclose(fd);

    return trie;
}

void info(Trie *trie, char nick[NICK_SIZE])
{
    User *user = trie_find(trie, nick);
    if (user == NULL || (user != NULL && apagado(user)))
    {
        printf("+ utilizador %s inexistente\n", nick);
        return;
    }

    printf("utilizador %s (%s)\n", nick, get_nome(user));
    printf("%d mensagens, %d seguidores, segue %d utilizadores\n", get_mensagens(user), get_seguidores(user), get_size(trie_find_segue(trie, nick)));
    

    print_lista_segue(trie_find_segue(trie, nick));
}


void read_message(Trie *trie, lista *lista)
{
    int diferenca = 0;

    node *user_atual = get_head(lista);
    //node *user_anterior = user_atual;
    while (user_atual != NULL)
    {
        User *user = trie_find(trie, get_list_nick(user_atual));
        if (apagado(user))
        {
            printf("utilizador %s desactivado\n", get_nick(user));
            node *user_a = get_next_node(user_atual);

            list_remove(lista, get_nick(user));
            user_atual = user_a;

        }
        else
        {
            diferenca = get_mensagens(user) - get_ultima_lida(user_atual); 
            char *nick = get_nick(user);
            char *nome = get_nome(user);
            switch (diferenca)
            {
                case 0:
                    printf("sem mensagens novas de %s (%s)\n", nick, nome);
                    break;
                
                case 1:
                    printf("mensagem nova de %s (%s): %d\n", nick, nome, get_mensagens(user));
                    add_ultima_lida(user_atual, get_mensagens(user));
                    break;
                
                default:
                    printf("mensagens novas de %s (%s): %d a %d\n", nick, nome, get_ultima_lida(user_atual) + 1, get_mensagens(user));
                    add_ultima_lida(user_atual, get_mensagens(user));
                    break;
            }
        }

        
        

        if(user != NULL && user_atual != NULL && strcmp(get_list_nick(user_atual), get_nick(user)) == 0)
        {
            add_lidas(user_atual, get_lidas(user_atual) + diferenca);
            user_atual =get_next_node(user_atual);
        }
       
    }
}
