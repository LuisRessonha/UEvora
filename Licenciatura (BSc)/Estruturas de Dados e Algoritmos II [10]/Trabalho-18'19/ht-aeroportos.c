#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

#include "ht-aeroportos.h"

#define FILE_NAME_AP "aeroportos.bin"
#define CELL_DISK_SIZE (sizeof(Aeroporto) + sizeof(short) + (sizeof(char)* MAX_COD_VOO)*MAX_LENGTH)

/* Construtor de uma celula da hashtable */
Cell *cell_new(Aeroporto *ap)
{
    Cell *cell = malloc(sizeof(*cell));

    cell->modificado = true;
    cell->ap = ap;
    cell->voos = list_new();

    return cell;
}


/* Libertar memoria de uma celula da hashtable */
void cell_destroy(Cell *cell)
{
    aeroporto_destroy(cell->ap);
    list_destroy(cell->voos);
    free(cell);
}

/* Escrever uma cell em disco */
void cell_write(Cell *cell, FILE *fd)
{ // nao guarda flag modificado
    fwrite(cell->ap, sizeof(Aeroporto), 1, fd);
    list_write(cell->voos, fd);
}

FILE *open_file_ap()
{
    FILE *fd = fopen(FILE_NAME_AP, "r+");

    if (fd != NULL)
        return fd;
    
    fd = fopen(FILE_NAME_AP, "w+");
    int zero = 0;
    fwrite(&zero, sizeof(int), 1, fd);

    return fd;
}

Cell *cell_read(FILE *fd, char cod_ap[MAX_COD_AP])
{
    Aeroporto *ap = aeroporto_new_empty();
    Cell *cell = cell_new(ap);
    fread(cell->ap, sizeof(Aeroporto), 1, fd);
    cell->modificado = false;

    if ( strcmp(cell->ap->cod_ap, "") == 0)
    {
        cell_destroy(cell);   
        return NULL;
    }
    list_destroy(cell->voos);
    cell->voos = list_read(fd);  


    return cell;
}

/* Construtor da hashtable */
Ht_ap *hashtable_ap_new(FILE *fd_ap)
{
    Ht_ap *hash = malloc(sizeof(*hash));

    int n;
    rewind(fd_ap);

    if (fread(&n, sizeof(int), 1, fd_ap) != 1)
        hash->n_ap = 0;
    else
        hash->n_ap = n;
    
    for (int i = 0; i < MAX_AEROPORTOS_HASH; i++)
    {
        hash->table[i] = NULL;
    }
    return hash;
}

void hashtable_ap_write_and_destroy(Ht_ap *hash, FILE *fd)
{  
    fseek(fd, 0, SEEK_SET);
    fwrite(&hash->n_ap, sizeof(int), 1, fd);

    for (int i = 0; i < MAX_AEROPORTOS_HASH; i++)
    {
        if (hash->table[i] != NULL)
        {   // NECESSITA DE SER OTIMIZADO
            if (hash->table[i]->modificado)
            {
                fseek(fd, sizeof(int), SEEK_SET);
                fseek(fd, CELL_DISK_SIZE * i, SEEK_CUR);
                cell_write(hash->table[i], fd);  
            }

            cell_destroy(hash->table[i]);
        }
    }
    free(hash);
}

/* Fonte: http://www.cse.yorku.ca/~oz/hash.html */
/* Função hash para strings, djb2 */
unsigned long hash_djb2_ap(char str[MAX_COD_AP])
{
    unsigned long hash = 5381;
        int c;

        while ( (c = *str++) )
            hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

        return hash;
}

Cell *get_cell_disk(Ht_ap *hash, FILE *fd, int pos, char cod_ap[MAX_COD_AP])
{
        fseek(fd, sizeof(hash->n_ap), SEEK_SET);
        fseek(fd, CELL_DISK_SIZE * pos, SEEK_CUR);
        return cell_read(fd, cod_ap);
        
}

/* Inserir aeroporto na hashtable */
bool hashtable_ap_insert(Ht_ap *hash, FILE *fd, Aeroporto *ap)
{
    int hash_code = hash_djb2_ap(ap->cod_ap) % MAX_AEROPORTOS_HASH;

    // quando ha colisao, hashing linear
    while (hash->table[hash_code] != NULL && strcmp(hash->table[hash_code]->ap->cod_ap, ap->cod_ap) != 0)
        hash_code = (hash_code + 1) % MAX_AEROPORTOS_HASH;

    if (hash->table[hash_code] == NULL)
    {
        // verificar no disco
        Cell *cell = get_cell_disk(hash, fd, hash_code, ap->cod_ap);
        
        while (cell != NULL && strcmp(cell->ap->cod_ap, ap->cod_ap) != 0)
        { // colisao em disco
            hash_code = (hash_code + 1) % MAX_AEROPORTOS_HASH;
            cell = get_cell_disk(hash, fd, hash_code, ap->cod_ap);
        }
        
        if (cell != NULL)
        {   // existe no disco, insere na memoria principal
            hash->table[hash_code] = cell;
            return false;
        }
        else
        {   
            ap->id = hash->n_ap;
            hash->table[hash_code] = cell_new(ap);
            hash->n_ap += 1;
            return true;
        }
        
    }
    else    // quer dizer que ja existe aeroporto com mesmo codigo
        return false;
}


/* Procura cell com determindado codigo */
Cell *hashtable_ap_get(Ht_ap *hash, FILE *fd,  char cod_ap[MAX_COD_AP])
{
    int hash_code = hash_djb2_ap(cod_ap) % MAX_AEROPORTOS_HASH;;

 
    
    while (hash->table[hash_code] == NULL || strcmp(hash->table[hash_code]->ap->cod_ap, cod_ap) != 0)
    {
        if (hash->table[hash_code] == NULL)
        {
            Cell *cell = get_cell_disk(hash, fd, hash_code, cod_ap);
            if (cell == NULL)
                break;
            
            hash->table[hash_code] = cell;

            if  (strcmp(cell->ap->cod_ap, cod_ap) == 0)
            {
                break;
            }
            
            
        }
        
        hash_code = (hash_code + 1) % MAX_AEROPORTOS_HASH; 
    }
 
 
 
 
 
 
 
    // // quando ha colisao, hashing linear
    // while(hash->table[hash_code] != NULL && strcmp(hash->table[hash_code]->ap->cod_ap, cod_ap) != 0)
    //     hash_code = (hash_code + 1) % MAX_AEROPORTOS_HASH;
 
    // // procurar no ficheiro
    // if (hash->table[hash_code] == NULL)
    // {
    //     Cell *cell = get_cell_disk(hash, fd, hash_code, cod_ap);

    //     while (cell != NULL && strcmp(cell->ap->cod_ap, cod_ap) != 0)
    //     {
    //         cell_destroy(cell);
    //         hash_code = (hash_code + 1) % MAX_AEROPORTOS_HASH;
    //         cell = get_cell_disk(hash, fd, hash_code, cod_ap);
    //     }

    //     hash->table[hash_code] = cell;
    // }        

     return hash->table[hash_code];
}





