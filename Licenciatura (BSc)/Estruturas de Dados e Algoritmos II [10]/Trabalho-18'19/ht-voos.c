#include <stdlib.h>
#include <string.h>

#include "ht-voos.h"

#define FILE_NAME_VOO "voos.bin"

/* Construtor da hashtable de voos */
Ht_voo *hashtable_voo_new()
{
    Ht_voo *hash = malloc(sizeof(*hash));

    for (int i = 0; i < MAX_VOOS; i++) 
        hash->table[i] = NULL;
    
    return hash;
}

FILE *open_file_voo()
{
    FILE *fd = fopen(FILE_NAME_VOO, "r+");

    if (fd != NULL)
        return fd;
    
    // se ficheiro nao existe, cria
    return fopen(FILE_NAME_VOO, "w+");
}

void write_voo_disk(FILE *fd, Voo *voo, int pos)
{
    fseek(fd, sizeof(Voo) * pos, SEEK_SET);

    fwrite(voo, sizeof(*voo), 1, fd);
}

/* Libertar memoria da hashtable */
void hashtable_voo_write_and_destroy(Ht_voo *hash, FILE *fd)
{  
    for (int i = 0; i < MAX_VOOS; i++)
    {
        if (hash->table[i] != NULL)
        {
            write_voo_disk(fd, hash->table[i], i);
            voo_destroy(hash->table[i]);
        }
    }

    free(hash);
}

// void hashtable_voo_read(Ht_voo *hash, FILE *fd)
// {
//     Voo *ler = voo_new_empty();
//     rewind(fd);
//     for (int i = 0; i < MAX_VOOS; i++)
//     {
//         if (fread(ler, sizeof(*ler), 1, fd) != 1 && strcmp(ler->cod_voo, "") == 0)
//             continue;
//         else
//         {
//             Voo *novo = voo_new(ler->cod_voo, ler->cod_ap_partida, ler->cod_ap_chegada, ler->h_partida, ler->m_partida, ler->duracao);
//             hash->table[i] = novo;
//         }
//     }

//     free(ler);
// }

Voo *get_voo_disk(Ht_voo *hash, FILE *fd, int pos, char cod_voo[MAX_COD_VOO])
{
    Voo *voo = voo_new_empty();

    fread(voo, sizeof(*voo), 1, fd);
    if (strcmp(voo->cod_voo, "") == 0)
    {
        voo_destroy(voo);
        return NULL;
    }
    return voo; 
}



/* Fonte: http://www.cse.yorku.ca/~oz/hash.html */
/* Função hash para strings, djb2 */
unsigned long hash_djb2_voo(char *str)
{
    unsigned long hash = 5381;
        int c;

        while ((c = *str++))
            hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

        return hash;
}

/* Inserir um voo na hashtable */
bool hashtable_voo_insert(Ht_voo *hash, FILE *fd_voo, Voo *voo)
{
    int hash_code = hash_djb2_voo(voo->cod_voo) % MAX_VOOS;

    // colisao, hashing linear
    while (hash->table[hash_code] != NULL && strncmp(hash->table[hash_code]->cod_voo, voo->cod_voo, MAX_COD_VOO) != 0)
        hash_code = (hash_code + 1) % MAX_VOOS;

    if (hash->table[hash_code] != NULL &&  hash->table[hash_code]->removido == 1)
    {
        free(hash->table[hash_code]);
        hash->table[hash_code] = NULL;
    }   

    if (hash->table[hash_code] == NULL)
    {
        hash->table[hash_code] = voo;
       // printf("cod_voo %s inserido em %d\n", hash->table[hash_code]->cod_voo, hash_code);
        
        //write_voo_disk(fd_voo, voo, hash_code);
        return true;
    }
    else    // quer dizer que ja existe voo com mesmo codigo
        return false;
}

int search_disk(Voo *voo, Ht_voo *hash, FILE *fd_voo, char cod_voo[MAX_COD_VOO], int hash_code)
{ 
    if (voo == NULL )
        return hash_code;
        
    hash->table[hash_code] = voo_new(voo->cod_voo, voo->cod_ap_partida, voo->cod_ap_chegada, voo->h_partida, voo->m_partida, voo->duracao);
    voo_destroy(voo);
    hash_code = (hash_code + 1) % MAX_VOOS; 
    voo = get_voo_disk(hash, fd_voo, hash_code, cod_voo);
    
    return hash_code;
}

Voo *hashtable_voo_get(Ht_voo *hash, FILE *fd_voo, char cod_voo[MAX_COD_VOO])
{
    int hash_code = hash_djb2_voo(cod_voo) % MAX_VOOS;

    Voo *voo;
    while (hash->table[hash_code] == NULL || strcmp(hash->table[hash_code]->cod_voo, cod_voo) != 0)
    {
        if (hash->table[hash_code] == NULL)
        {
            voo = get_voo_disk(hash, fd_voo, hash_code, cod_voo);
            if (voo == NULL)
                break;
            
            hash->table[hash_code] = voo_new(voo->cod_voo, voo->cod_ap_partida, voo->cod_ap_chegada, voo->h_partida, voo->m_partida, voo->duracao);

            if  (strcmp(voo->cod_voo, cod_voo) == 0)
            {
                voo_destroy(voo);
                break;
            }
            voo_destroy(voo);
        }
        
        hash_code = (hash_code + 1) % MAX_VOOS; 
    }





        // hash_code = search_disk(voo, hash, fd_voo, cod_voo, hash_code);

        // if (hash->table[hash_code])

        // if (voo != NULL && strcmp(voo->cod_voo, cod_voo) == 0)
        // {
        //     hash->table[hash_code] = voo_new(voo->cod_voo, voo->cod_ap_partida, voo->cod_ap_chegada, voo->h_partida, voo->m_partida, voo->duracao);
        //     voo_destroy(voo);
        //     break;
        // }
    

    // {
    //     if (voo != NULL && strncmp(voo->cod_voo, cod_voo, MAX_COD_VOO) != 0)
    //     {
    //         hash->table[hash_code] = voo_new(voo->cod_voo, voo->cod_ap_partida, voo->cod_ap_chegada, voo->h_partida, voo->m_partida, voo->duracao);
    //         voo_destroy(voo);
    //         hash_code = (hash_code + 1) % MAX_VOOS; 
    //         voo = get_voo_disk(hash, fd_voo, hash_code, cod_voo);
    //     }

    //     if (voo != NULL && strncmp(voo->cod_voo, cod_voo, MAX_COD_VOO) == 0)
    //         hash->table[hash_code] = voo;
        
    //     if (voo == NULL && hash->table[hash_code] == NULL)
    //         break;
    
    

//    printf("cod_voo %s getteed em %d\n", cod_voo, hash_code);
     if (hash->table[hash_code] != NULL && hash->table[hash_code]->removido == 1)
             return NULL;

    return hash->table[hash_code];
}

/* Remover um voo da hashtable
   Retorna aeroporto de origem para o remover da hashtable*/
bool hashtable_voo_remove(Ht_voo *hash, Ht_ap *hash_ap, FILE *fd_ap, FILE *fd_voo, char cod_voo[MAX_COD_VOO])
{
    int hash_code = hash_djb2_voo(cod_voo) % MAX_VOOS;

    // caso haja colisao, hashing linear
    while (hash->table[hash_code] != NULL && strncmp(hash->table[hash_code]->cod_voo, cod_voo, MAX_COD_VOO) != 0)
        hash_code = (hash_code + 1) % MAX_VOOS;
    
    // voo a remover nao existe
    if (hash->table[hash_code] == NULL)
            return false;
   
    Cell *cell = hashtable_ap_get(hash_ap, fd_ap, hash->table[hash_code]->cod_ap_partida);
    list_remove(cell->voos, cod_voo);
    cell->modificado = true;

    // Voo *remover = voo_new_empty();
    // write_voo_disk(fd_voo, remover, hash_code);
    // voo_destroy(remover);
    // voo_destroy(hash->table[hash_code]);
    // hash->table[hash_code] = NULL;

    hash->table[hash_code]->removido = 1;
    return true;
}
