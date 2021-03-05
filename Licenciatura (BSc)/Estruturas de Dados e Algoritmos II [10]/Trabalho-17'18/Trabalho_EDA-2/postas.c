#include <stdio.h>

#include "new_ht.h"


void criar_utilizador(Hashtable *ht, FILE *f_users, FILE *f_hash,  char nick[NICK_SIZE], char nome[NOME_SIZE])
{
    int index = search_ptr(ht, nick);
    if (index == -1)
    {
        insert_item(f_hash, ht, new_user(f_users, nick, nome), nick);
        printf("+ utilizador %s criado\n", nick);
    }
    else
        printf("+ nick %s usado previamente\n", nick);
}


void remover_utilizador(Hashtable *ht, FILE *fd, char nick[NICK_SIZE])
{
    int index = search_ptr(ht, nick);
    if (index != -1 && apagar_user(fd, index))
        printf("+ utilizador %s removido\n", nick);
    else
        printf("+ utilizador %s inexistente\n", nick);
}


void seguir_utilizador(Hashtable *ht, FILE *fd, char nick1[NICK_SIZE], char nick2[NICK_SIZE])
{
   int index1 = search_ptr(ht, nick1);
   

    if (index1 == -1 || (index1 != -1 && apagado(fd, index1)))
    {
        printf("+ utilizador %s inexistente\n", nick1);
        return;
    }
  //  lista *seguem_user1 = trie_find_segue(trie, nick1);

    int index2 = search_ptr(ht, nick2);
    if (index2 == -1 || (index2 != -1 && apagado(fd, index2)))
    {
        printf("+ utilizador %s inexistente\n", nick2);
        return;
    }

    User *u1 = read_user(fd, index1);
    User *u2 = read_user(fd, index2);
   // printf("User: %s\n", u1->nome);
    int e = inserir_seguido(u1->info, nick2, u2->mensagens);
    //printf("User: %s\n", u1->nome);

    switch (e)
    { 
        case -1:
            printf("+ utilizador %s segue o limite\n", nick1);
            break;

        case -2:
            printf("+ utilizador %s segue %s\n", nick1, nick2);
            break;

        default:
            u1->info->segue[e]->pointer = search_ptr(ht, nick2);
            
            write_user(fd, u1, index1);
            printf("+ %s passou a seguir %s\n", nick1, nick2);
            adicionar_seguidor(fd, index2);
            break;
    }

    destroy_user(u1);
    destroy_user(u2);
    
}


void deixar_seguir(Hashtable *ht, FILE *fd, char nick1[NICK_SIZE], char nick2[NICK_SIZE])
{
    int index1 = search_ptr(ht, nick1);

    if (index1 == -1 || (index1 != -1 && apagado(fd, index1)))
    {
        printf("+ utilizador %s inexistente\n", nick1);
        return;
    }

    int index2 = search_ptr(ht, nick2);
    if (index2 == -1 || (index2 != -1 && apagado(fd, index2)))
    {
        printf("+ utilizador %s inexistente\n", nick2);
        return;
    }

    User *u1 = read_user(fd, index1);

    if (u1->info != NULL && remover_seguido(u1->info, nick2))
    {
        write_user_D(fd, u1, index1);
        printf("+ %s deixou de seguir %s\n", nick1, nick2);
        remover_seguidor(fd, index2);
    }
    else
        printf("+ utilizador %s nao segue %s\n", nick1, nick2);
    
    destroy_user(u1);

}

void enviar_mensagem(Hashtable *ht, FILE *fd, char nick[NICK_SIZE])
{
    int index = search_ptr(ht, nick);


    User *u = read_user(fd, index);

    if (u != NULL && !u->apagado)
    {
        adicionar_mensagem(u);
        write_user(fd, u, index);
    }
    else
        printf("+ utilizador %s inexistente\n", nick);

    destroy_user(u);
}

void ler_mensagens(Hashtable *ht, FILE *fd, char nick[NICK_SIZE])
{
    int index = search_ptr(ht, nick);

    if (index == -1)
    {
        printf("+ utilizador %s inexistente\n", nick);
        return;
    }
    
    User *u = read_user(fd, index);
    if (u->apagado)
    {
        destroy_user(u);
        printf("+ utilizador %s inexistente\n", nick);
        return;
    }
    
    info_segue *info = u->info;
    if (seguidos_empty(info))
    {
        destroy_user(u);
        printf("+ utilizador %s sem seguidos\n", nick);
        return;
    }

    ler_mensagem(info, fd);
    write_user(fd, u, index);
    destroy_user(u);
}

void informacao(Hashtable *ht, FILE *fd, char nick[NICK_SIZE])
{
    int index = search_ptr(ht, nick);

    if (index == -1)
    {
        printf("+ utilizador %s inexistente\n", nick);
        return;
    }

    User *u = read_user(fd, index);
    if (u->apagado)
    {
        destroy_user(u);
        printf("+ utilizador %s inexistente\n", nick);
        return;
    }
    printf("utilizador %s (%s)\n", u->nick, u->nome);
    printf("%d mensagens, %d seguidores, segue %d utilizadores\n", u->mensagens, u->seguidores, u->info->size);
    print_seguidos(u->info);
    destroy_user(u);
}


FILE *open_users(char *fich)
{
    FILE *fd = fopen(fich, "r+");

    if (fd == NULL)
        fd = fopen(fich, "w+");

    return fd;
}


int main()
{

    Hashtable *ht = read_hashtable();
    FILE *f_users = open_users(FICH_USERS);
    FILE *f_hash = open_hashfile();

    char cod = '-';
    char nick1[NICK_SIZE], nick2[NICK_SIZE];
    char nome[NOME_SIZE];

    while (cod != 'X')
    {
        scanf("%c", &cod);
        switch(cod)
        {
            case 'U':
                scanf(" %s %[^\n]", nick1, nome);
                criar_utilizador(ht, f_users, f_hash, nick1, nome);
                break;

            case 'R':
                scanf(" %s", nick1);
                remover_utilizador(ht, f_users, nick1);
                break;

            case 'S':
                scanf(" %s %[^\n]", nick1, nick2);
                seguir_utilizador(ht, f_users, nick1, nick2);
                break;

            case 'D':
                scanf(" %s %[^\n]", nick1, nick2);
                deixar_seguir(ht, f_users, nick1, nick2);
                break;
           
            case 'P':
                scanf("%s", nick1);
                enviar_mensagem(ht, f_users, nick1);
                break;

            case 'L':
                scanf("%s", nick1);
                ler_mensagens(ht, f_users, nick1);
                break;

            case 'I':
                scanf("%s", nick1);
                informacao(ht, f_users, nick1);
                break;

        }
    }


    fclose(f_users);
    fclose(f_hash);
    destroy_hashtable(ht);


}