#include "user.h"

/*
    destroy_user:
        - Recebe um user e liberta a memória

*/
void destroy_user(User *u)
{   
    destroy_info(u->info);  // liberta a memória do array de quem segue
    free(u);
}


/*
    read_user:
        - Recebe o descritor do ficheiro de users e um index
        - Lê do ficheiro de users o user correspondente ao index
        - Devolve o apontador do user lido
*/
User *read_user(FILE *fd, int index)
{
    fseek(fd, index, SEEK_SET);     // vai para a posição do user que se quer
    User *u = malloc(sizeof(User)); // aloca espaço

    // lê dados do user
    fread(u->nick, NICK_SIZE * sizeof(char), 1, fd);
    fread(u->nome, NOME_SIZE * sizeof(char), 1, fd);
    fread(&u->apagado, sizeof(bool), 1, fd);
    fread(&u->mensagens, sizeof(int), 1, fd);
    fread(&u->seguidores, sizeof(int), 1, fd);
    u->info = read_all_seguidos(fd);    // lê array de quem segue o user u

    return u;
}

void write_user(FILE *fd, User *u, int index)
{
    fseek(fd, index, SEEK_SET);

    fwrite(u->nick, NICK_SIZE * sizeof(char), 1, fd);
    fwrite(u->nome, NOME_SIZE * sizeof(char), 1, fd);
    fwrite(&u->apagado, sizeof(bool), 1, fd);
    fwrite(&u->mensagens, sizeof(int), 1, fd);
    fwrite(&u->seguidores, sizeof(int), 1, fd);
    write_seguidos(fd, u->info);
}

void write_user_D(FILE *fd, User *u, int index)
{
    fseek(fd, index, SEEK_SET);

    fwrite(u->nick, NICK_SIZE * sizeof(char), 1, fd);
    fwrite(u->nome, NOME_SIZE * sizeof(char), 1, fd);
    fwrite(&u->apagado, sizeof(bool), 1, fd);
    fwrite(&u->mensagens, sizeof(int), 1, fd);
    fwrite(&u->seguidores, sizeof(int), 1, fd);
    write_seguidos_D(fd, u->info);
}

void write_all_user(FILE *fd, User *u, int index)
{
    fseek(fd, index, SEEK_SET);

    fwrite(u->nick, NICK_SIZE * sizeof(char), 1, fd);
    fwrite(u->nome, NOME_SIZE * sizeof(char), 1, fd);
    fwrite(&u->apagado, sizeof(bool), 1, fd);
    fwrite(&u->mensagens, sizeof(int), 1, fd);
    fwrite(&u->seguidores, sizeof(int), 1, fd);
    write_all_seguidos(fd, u->info);

}

int new_user(FILE *fd, char nick[NICK_SIZE], char nome[NOME_SIZE])
{
    User *u = malloc(sizeof(User));
    strcpy(u->nick, nick);
    strcpy(u->nome, nome);
    u->apagado = false;
    u->mensagens = 0;
    u->seguidores = 0;
    u->info = new_seguidos();


    fseek(fd, 0, SEEK_END);
    int index = ftell(fd);
    write_all_user(fd, u, index);
    
    destroy_user(u);

    return index;
}


bool apagar_user(FILE *fd, int index)
{
    User *u = read_user(fd, index);

    if (u->apagado)
    {
        destroy_user(u);
        return false;   
    }   
    u->apagado = true;

    write_user(fd, u, index);
    destroy_user(u);

    return true;
}

int get_mensagens(FILE *fd, int index)
{
    User *u = malloc(sizeof(User));
    write_user(fd, u, index);

    int mensagens = u->mensagens;

    destroy_user(u);

    return mensagens;
}

char *get_nome(FILE *fd, int index)
{
    User *u = read_user(fd, index);
    char *nome = u->nome;

    destroy_user(u);
    return nome;
}

info_segue *get_info(FILE *fd, int index)
{
    User *u = read_user(fd, index);
    info_segue *info = u->info;

    destroy_user(u);
    return info;
}

bool apagado(FILE *fd, int index)
{
    User *u = read_user(fd, index);

    bool a = u->apagado;

    destroy_user(u);

    return a;
}

void adicionar_seguidor(FILE *fd, int index)
{
    User *u = read_user(fd, index);
    u->seguidores = u->seguidores + 1;

    write_user(fd, u, index);

    destroy_user(u);
}

void remover_seguidor(FILE *fd, int index)
{
    User *u = read_user(fd, index);
    u->seguidores = u->seguidores - 1;

    write_user(fd, u, index);
    
    destroy_user(u);
}

void adicionar_mensagem(User *u)
{
    u->mensagens = u->mensagens + 1;
}



/*  ler_mensagem:
        - Recebe como argumento:
            * A informacoa do array
            * O ficheiro onde as informacoes esto guardadas em disco
            * O array onde estao guardados os users seguidos
        - Nao tem retorno
        - Percorre o array e faz print de todos os seguidos assim como a informacao
       sobre as novas mensagens destes
*/
void ler_mensagem(info_segue *info, FILE *fd)
{
    char nick[NICK_SIZE];
    char nome[NOME_SIZE];
    int diferenca = 0;

    for (int i = 0; i < info->size; i++)
    {
        User *u = read_user(fd, info->segue[i]->pointer);
        strcpy(nick, u->nick);
        strcpy(nome, u->nome); // implementar get_nome
        diferenca = u->mensagens - info->segue[i]->ultima_lida;

        // Se algum seguido foi destivado e removido do array de seguidos
        if (apagado(fd, info->segue[i]->pointer))
        {  
            destroy_user(u);
            printf("utilizador %s desactivado\n", nick);
            remover_seguido(info, nick);
            i = i-1;
        }
        else
        {
            switch (diferenca)
            {
                /* Quando o user nao enviou nenhuma mensagem desde o momento 
                 que comecou a ser seguido */
                case 0:
                    printf("sem mensagens novas de %s (%s)\n", nick, nome);
                    break;

                /* Quando o user enviou apenas uma mensagem desde o momento 
                 que comecou a ser seguido */
                case 1:
                    printf("mensagem nova de %s (%s): %d\n", nick, nome, u->mensagens);
                    info->segue[i]->ultima_lida = u->mensagens;
                    break;

                /* Quando o user enviou mais do que uma mensagem desde o  
                 momento que comecou a ser seguido */
                default:
                    printf("mensagens novas de %s (%s): %d a %d\n", nick, nome, info->segue[i]->ultima_lida + 1, u->mensagens);
                    info->segue[i]->ultima_lida = u->mensagens;
                    break;
            }

             destroy_user(u);
        }
       
    }
}
