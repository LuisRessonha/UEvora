#include "seguidos.h"

void destroy_info(info_segue *info)
{
    for (int i = 0; i < MAX_SEGUIDOS; i++)
    {
        if (info->segue[i] != NULL)
            free(info->segue[i]);
    }

    free(info);
}


void write_node_seg(FILE *fd, node_seg *n)
{
    fwrite(n->nick, NICK_SIZE * sizeof(char), 1, fd);
    fwrite(&n->ultima_lida, sizeof(int), 1, fd);
    fwrite(&n->lidas, sizeof(int), 1, fd);
    fwrite(&n->pointer, sizeof(int), 1, fd);
}

void write_all_seguidos(FILE *fd, info_segue *info)
{
    fwrite(&info->size, sizeof(int), 1, fd);

    for (int i = 0; i < MAX_SEGUIDOS; i++)
        write_node_seg(fd, info->segue[i]);
}

void write_seguidos(FILE *fd, info_segue *info)
{
    fwrite(&info->size, sizeof(int), 1, fd);

    for (int i = 0; i < info->size; i++)
        write_node_seg(fd, info->segue[i]);
}

void write_seguidos_D(FILE *fd, info_segue *info)
{
    fwrite(&info->size, sizeof(int), 1, fd);

    for (int i = 0; i < info->size + 1; i++)
        write_node_seg(fd, info->segue[i]);
}

node_seg *read_node_seg(FILE *fd)
{
    node_seg *n = malloc(sizeof(node_seg));
    fread(n->nick, NICK_SIZE * sizeof(char), 1, fd);
    fread(&n->ultima_lida, sizeof(int), 1, fd);
    fread(&n->lidas, sizeof(int), 1, fd);
    fread(&n->pointer, sizeof(int), 1, fd);

    return n;
}

info_segue *read_seguidos(FILE *fd)
{
    info_segue *info = malloc(sizeof(info_segue));
    fread(&info->size, sizeof(int), 1, fd);

    for (int i = 0; i < info->size + 1; i++)
        info->segue[i] = read_node_seg(fd);

    return info;
}

info_segue *read_all_seguidos(FILE *fd)
{
    info_segue *info = malloc(sizeof(info_segue));
    fread(&info->size, sizeof(int), 1, fd);

    for (int i = 0; i < MAX_SEGUIDOS; i++)
        info->segue[i] = read_node_seg(fd);

    return info;
}

node_seg *new_node_seg()
{
    node_seg *n = malloc(sizeof(node_seg));

    strcpy(n->nick, "");
    n->ultima_lida = -1;
    n->lidas = -1;
    n->pointer = -1;

    return n;
}

info_segue *new_seguidos()
{
    info_segue *info = malloc(sizeof(info_segue));

    info->size = 0;

    for (int i = 0; i < MAX_SEGUIDOS; i++)
        info->segue[i] = new_node_seg();

    return info;
}




/*  seguidos_empty:
        - Recebe como argumento a informacao do array de seguidos
        - Tem retorno "true" ou "false"
        - Verifica se o utilizador segue algum user (true) ou ainda nao (false)
*/
bool seguidos_empty(info_segue *info)
{
    if (info->size > 0)
        return false;
    
    return true;
}

/*  seguidos_full:
        - Recebe como argumento a informacao do array
        - Tem retorno "true" ou "false"
        - Verifica se o utilizador atingiu o maximo de users 
       que pode seguir (true) ou ainda nao (false)
*/
bool seguidos_full(info_segue *info)
{
    if (info->size >= MAX_SEGUIDOS)
        return true;
    
    return false;
}

int procura_bin(char nick[NICK_SIZE], info_segue *info, int inicio, int fim)
{
    int meio = (inicio + fim) / 2;

    char nick_meio[NICK_SIZE];
    strcpy(nick_meio, info->segue[meio]->nick);
    int comp = strcmp(nick, nick_meio);

    if (inicio > fim)
        return meio + 1;

    if (comp > 0)
        return procura_bin(nick, info, meio + 1, fim);

    if (comp < 0)
        return procura_bin(nick, info, inicio, meio - 1);
 
    return meio;
}

int find_seguido(info_segue *info, char nick[NICK_SIZE])
{
    if (seguidos_empty(info))
        return 0;

    return procura_bin(nick, info, 0, info->size - 1);
}

/*  print_seguidos:
        - Recebe como argumento a informacao do array
        - Nao tem retorno
        - Faz print de todos os users que segue (se seguir)
       assim como a ultima mensagem leu desse user
*/
void print_seguidos(info_segue *info)
{
    // Verifica se tem seguidos
    if (seguidos_empty(info))
        return;
    
    // percorre o array e faz print de cada user seguido
    for (int i = 0; i < info->size; i++)
    {
        printf("%s (%d lidas)\n", info->segue[i]->nick, info->segue[i]->ultima_lida);
    }
}

void set_info(info_segue *info, int pos, char nick[NICK_SIZE], int msg)
{
    info->segue[pos]->lidas = 0;
    strcpy(info->segue[pos]->nick, nick);
    info->segue[pos]->ultima_lida = msg;
    info->size = info->size + 1;
}

void clone_node(node_seg *n1, node_seg *n2)
{
    n1->lidas = n2->lidas;
    n1->pointer = n2->pointer;
    n1->ultima_lida = n2->ultima_lida;
    strcpy(n1->nick, n2->nick);
}


int inserir_seguido(info_segue *info , char nick[NICK_SIZE], int ultima_lida)
{
    int pos = find_seguido(info, nick);

    if (seguidos_empty(info))
    {
        strcpy(info->segue[pos]->nick, nick);
        info->segue[pos]->lidas = 0;
        info->segue[pos]->ultima_lida = ultima_lida;
        info->size = info->size + 1;
        return pos; //foi inserido com sucesso
    }


    if (pos < info->size && strcmp(info->segue[pos]->nick, nick) == 0)
    {
        return -2;
    }

    if (seguidos_full(info))
        return -1; 

    int comp = 0;
    char nick2[NICK_SIZE];
    pos = 0;
    while(pos < info->size)
    {
        strcpy(nick2, info->segue[pos]->nick);
        comp = strcmp(nick, nick2);

        if (comp == 0)
            return -2; // ja foi inserido anteriormente
        
        if (comp < 0)
        {
            // arrasta todos os seguidos uma posicao para a frente
            for (int i = info->size - 1; i >= pos; i--) 
            {
                clone_node(info->segue[i+1], info->segue[i]);
            }

            set_info(info, pos, nick, ultima_lida);

            return pos; //foi inserido com sucesso 
        }
        pos++;
    }
    
    strcpy(info->segue[pos]->nick, nick);
    info->segue[pos]->lidas = 0;
    info->segue[pos]->ultima_lida = ultima_lida;
    info->size = info->size + 1;
    return pos; //foi inserido com sucesso
}

/*  inserir_seguido:
        - Recebe como argumento:
            * A hashtable onde estao guardados todos os users
            * A informacoa do array
            * O nick do user que se pretende seguir
            * O index (pointer) do user que se pretende seguir
            * O numero de mensagens enviadas pelo user que se pretende seguir
        - Retorna "0", "1" ou "2" caso comece a seguir, ja segue o maximo ou 
       esse user ja e seguido (repetivamente)
        - Percorre o array, verifica se o user em questao ja e seguido, se 
       ja segue o limite e, se possivel, adiciona o novo seguido por ordem lexicografica

int inserir_seguido(info_segue *info, char nick[NICK_SIZE], int ultima_lida)
{
    int comp = 0;
    char nick2[NICK_SIZE]; // nick que ja e seguido (usado para comprarar)
    int pos = 0;

    // percorre todas as posicoes (ocupadas) do array de seguidos
    while(pos < info->size)
    {
        strcpy(nick2, info->segue[pos]->nick);
        comp = strcmp(nick, nick2);

        if (comp == 0)
            return -2; // ja foi inserido anteriormente
        
        if (comp < 0)
        {
            // verifica se e possvel adicionar mais seguidos
            if (seguidos_full(info))
                return -1; 

            // arrasta todos os seguidos uma posicao para a frente
            for (int i = info->size - 1; i >= pos; i--) 
            {
                clone_node(info->segue[i+1], info->segue[i]);
            }

            set_info(info, pos, nick, ultima_lida);

            return pos; //foi inserido com sucesso 
        }
        pos++;
    }

    // verifica se e possvel adicionar mais seguidos
    if (seguidos_full(info))    
        return -1; // chegou ao n max de seguidores


    strcpy(info->segue[pos]->nick, nick);
    info->segue[pos]->lidas = 0;
    info->segue[pos]->ultima_lida = ultima_lida;
    info->size = info->size + 1;
    return pos; //foi inserido com sucesso
}*/

bool remover_seguido(info_segue *info, char nick[NICK_SIZE])
{
    if (seguidos_empty(info))
        return false;

    int pos = find_seguido(info, nick);

    if (seguidos_full(info))
    {
        if(strcmp(info->segue[pos]->nick, nick) == 0)
        {   
        for (; pos < info->size - 1; pos++)
            clone_node(info->segue[pos], info->segue[pos+1]);

        info->size = info->size - 1; // Diminui o numero de seguido (na informacao do array)
        return true; // nick removido
        }
    }

    //printf("\n\npos: %d\nsize: %d\n\n", pos, info->size);
/*
    if(pos == info->size - 1)
    {
        info->size = info->size - 1; // Diminui o numero de seguido (na informacao do array)
        return true; // nick removido
    }*/
    if(strcmp(info->segue[pos]->nick, nick) == 0)
    {   
        for (; pos < info->size; pos++)
            clone_node(info->segue[pos], info->segue[pos+1]);



        info->size = info->size - 1; // Diminui o numero de seguido (na informacao do array)
        return true; // nick removido
    }

    return false;
}

/*  remover_seguido:
        - Recebe como argumento:
            * O array onde estao guardados os users seguidos
            * O nick do user que se pretende deixar de seguir
        - Retorna "true" ou "false"
        - Percorre o array para procurar o nick do user que se quer deixar de seguir,
       se for encontrado e removido (retorno: "true")

bool remover_seguido (info_segue *info, char nick[NICK_SIZE])
{
    // Verifica se existem seguidos
    if (seguidos_empty(info))
        return false; // nao segue ninguem (nenhum nick foi removido) 

    int comp = 0;
    int pos = 0;
    char nick2[NICK_SIZE]; // nick que ja e seguido (usado para comprarar)
    
    // Percorre o array para procurar o nick (que se quer remover)
    for (int i = 0; i < info->size; i++)
    {
        strcpy(nick2, info->segue[i]->nick);
        comp = strcmp(nick, nick2);

        // Quando o nick e encontrado
        if (comp == 0)
        {
             Percorre o array a partir da posicao onde o nick foi encontrado
               e arrasta os nicks seguintes uma posicao pra tras
            
            for (pos = i; pos < info->size - 1; pos++)
            {
                clone_node(info->segue[pos], info->segue[pos+1]);
            }
            info->size = info->size - 1; // Diminui o numero de seguido (na informacao do array)
            return true; // nick removido
        }

        // Quando o nick a remover e maior do que o que esta a ser comprado
        if (comp < 0)
            return false; // nenhum nick foi removido (o nick nao existe na lista)
    }

    
    return false; // nenhum nick foi removido
}
*/


/*  numero_seguidos:
        - Recebe como argumento a informacoa do array
        - Retorna o numero de seguidos
*/
int numero_seguidos(info_segue *info)
{
    return info->size;
}
