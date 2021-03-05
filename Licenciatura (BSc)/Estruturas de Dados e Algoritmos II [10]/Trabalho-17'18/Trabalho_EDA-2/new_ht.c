#include "new_ht.h"

/* Estrutura de um node da hashtable */
typedef struct hashtable_item {
    char nick[NICK_SIZE];   // nick de um user
    bool ocupado;           // indica se celula está ocupada ou não
    int ptr;                // pointer para posição de user num ficheiro
} Item;

/* Estrutura da hashtable */
typedef struct hashtable {
    Item *items[MAX_USERS]; // tabela de hash 
    int size;               // Nº de elementos da tabela 
} Hashtable;

/*
    open_hashfile:
        - Não recebe nada como argumento
        - Retorna o descritor do ficheiro da hash
*/
FILE *open_hashfile()
{
    FILE *fd = fopen(FICH_HT, "r+");

    if (fd == NULL)     // se ficheiro não exista, cria-o
        fd = fopen(FICH_HT, "w+");

    return fd;
}


Item *new_item()
{
    Item *item = malloc(sizeof(Item));

    item->ocupado = false;
    item->ptr = -1;
    strcpy(item->nick, "vazio");
    
    return item;
}

Hashtable *new_hashtable()
{
    
    Hashtable *ht = malloc(sizeof(Hashtable));

    ht->size = 0;

    
    return ht;
}

void destroy_hashtable(Hashtable *ht)
{
    // aproveita para guardar size no ficheiro...
    FILE *fd = open_hashfile();
    fwrite(&ht->size, sizeof(int), 1, fd);
    fclose(fd);

/*
    for (int i = 0; i < MAX_USERS; i++)
    {
        if (ht->items[i] != NULL)
            {
                free(ht->items[i]);
            }
    }
*/
    free(ht);
}



unsigned long long get_key(char nick[NICK_SIZE])
{
    unsigned long long h = 5381;
    while (*(nick++))
        h = ((h << 5) + h) + (*nick);

    return h;
}


int hash(char nick[NICK_SIZE])
{
    return get_key(nick) % MAX_USERS;
}

int double_hash(char nick[NICK_SIZE])
{
    return MAX_PRIME - (get_key(nick) % MAX_PRIME);
}


Hashtable *read_hashtable()
{
    FILE *fd = open_hashfile();
    Hashtable *ht = new_hashtable();

    // ler tamanho de hashtable
    if (fread(&ht->size, sizeof(int), 1, fd) != 1) 
    { //se não haver size é porque não ha hashtable em ficheiro
        ht->size = 0;
        fclose(fd);
        return ht;
    }
      
    int hashcode, hashcode2;
    for (int i = 0; i < ht->size; i++)
    {
        char nick[NICK_SIZE];
        fread(nick, NICK_SIZE * sizeof(char), 1, fd);

        // obter hashcode de nick 
        hashcode = hash(nick);
        hashcode2 = double_hash(nick);

        while (ht->items[hashcode] != NULL && ht->items[hashcode]->ocupado)
            hashcode = (hashcode + hashcode2) % MAX_USERS;
        
        if (ht->items[hashcode] == NULL)
            ht->items[hashcode] =new_item();

        strcpy(ht->items[hashcode]->nick, nick);
        fread(&ht->items[hashcode]->ocupado, sizeof(bool), 1, fd);
        fread(&ht->items[hashcode]->ptr, sizeof(int), 1, fd);
    }

    fclose(fd);
    return ht;
}


void insert_item(FILE *fd, Hashtable *ht, int ptr, char nick[NICK_SIZE])
{  
    int hashcode = hash(nick);
    int hashcode2 = double_hash(nick);
        
    while (ht->items[hashcode] != NULL && ht->items[hashcode]->ocupado)
        hashcode = (hashcode + hashcode2) % MAX_USERS;
    
    if (ht->items[hashcode] == NULL)
        ht->items[hashcode] = new_item();


    ht->items[hashcode]->ptr = ptr;
    strcpy(ht->items[hashcode]->nick, nick);
    ht->items[hashcode]->ocupado = true;

    ht->size = ht->size + 1;


    // escrever no ficheiro
    rewind(fd);
    int size;
    // se for a primeira vez que abre o ficheiro da hashtable e nao tem size
    if (fread(&size, sizeof(int), 1, fd) != 1)
        fwrite(&ht->size, sizeof(int), 1, fd);

    fseek(fd, 0, SEEK_END);

    fwrite(ht->items[hashcode]->nick, NICK_SIZE * sizeof(char), 1, fd);
    fwrite(&ht->items[hashcode]->ocupado, sizeof(bool), 1, fd);
    fwrite(&ht->items[hashcode]->ptr, sizeof(int), 1, fd);
}


int search_ptr(Hashtable *ht, char nick[NICK_SIZE])
{
    int hashcode = hash(nick);
    int hashcode2 = double_hash(nick);

    while (ht->items[hashcode] != NULL && ht->items[hashcode]->ocupado)
    {
        if (strcmp(ht->items[hashcode]->nick, nick) == 0)
            return ht->items[hashcode]->ptr;

        hashcode = (hashcode + hashcode2) % MAX_USERS;
    }
    
    return -1;

}



