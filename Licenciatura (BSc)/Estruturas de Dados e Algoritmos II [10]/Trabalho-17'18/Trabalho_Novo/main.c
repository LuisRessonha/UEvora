#include <stdio.h>

#include "ht.h"


void criar_utilizador()
{}


void remover_utilizador()
{}


void seguir_utilizador()
{}


void deixar_seguir()
{}

void enviar_mensagem()
{}

void ler_mensagens()
{}

void informacao()
{}


FILE *open_users(char *fich)
{
    FILE *fd = fopen(fich, "r+");

    if (fd == NULL)
        fd = fopen(fich, "w+");

    return fd;
}


int main()
{
    
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