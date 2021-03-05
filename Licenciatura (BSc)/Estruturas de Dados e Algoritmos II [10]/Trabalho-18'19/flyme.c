#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "ht-voos.h"
#include "dijkstra.h"

#define OP_SIZE 3

void introduzir_aeroporto(Ht_ap *hash_ap, FILE *fd_ap)
{
    char cod_ap[MAX_COD_AP];
    short h_fuso;
    short m_fuso;
    scanf(" %s %hd:%hd", cod_ap, &h_fuso, &m_fuso);
    Aeroporto *ap = aeroporto_new(cod_ap, h_fuso, m_fuso);

    if (hashtable_ap_insert(hash_ap, fd_ap, ap))
        printf("+ novo aeroporto %s\n", cod_ap);
    else
    {
        printf("+ aeroporto %s existe\n", cod_ap);
        free(ap);
    }
}

void introduzir_voo(Ht_voo *hash_voo, Ht_ap *hash_ap, FILE *fd_voo, FILE *fd_ap)
{
    char cod_voo[MAX_COD_VOO];
    char cod_ap_partida[MAX_COD_AP];
    char cod_ap_chegada[MAX_COD_AP];
    short h_par;
    short m_par;
    short dur;
    scanf(" %s %s %s %hd:%hd %hd", cod_voo, cod_ap_partida, cod_ap_chegada, &h_par, &m_par, &dur);
    Voo *voo = voo_new(cod_voo, cod_ap_partida, cod_ap_chegada, h_par, m_par, dur);
    Voo *getted = hashtable_voo_get(hash_voo, fd_voo, cod_voo);
    
     if (getted != NULL)    
        printf("+ voo %s existe\n", cod_voo);

     else if (hashtable_ap_get(hash_ap, fd_ap, cod_ap_partida) == NULL)
        printf("+ aeroporto %s desconhecido\n", cod_ap_partida);

    else if (hashtable_ap_get(hash_ap, fd_ap, cod_ap_chegada) == NULL)
        printf("+ aeroporto %s desconhecido\n", cod_ap_chegada);
    
    else if (hashtable_voo_insert(hash_voo, fd_voo, voo))
    {
        Cell *cell = hashtable_ap_get(hash_ap, fd_ap, cod_ap_partida);
        list_insert(cell->voos, cod_voo);
        cell->modificado = true;
        printf("+ novo voo %s\n", cod_voo);
        return;
    }

    free(voo);
    
}

void remover_voo(Ht_voo *hash_voo, Ht_ap *hash_ap, FILE *fd_voo, FILE *fd_ap)
{
    char cod_voo[MAX_COD_VOO];
    scanf(" %s", cod_voo);
    Voo *voo = hashtable_voo_get(hash_voo, fd_voo, cod_voo);
    if (voo == NULL || voo->removido == 1)
    {
        printf("+ voo %s inexistente\n", cod_voo);
        return;
    }

    hashtable_voo_remove(hash_voo, hash_ap, fd_ap, fd_voo, cod_voo);
    printf("+ voo %s removido\n", cod_voo);
}

void calcular_viagem(Ht_voo *hash_voo, FILE *fd_voo, Ht_ap *hash_ap, FILE *fd_ap)
{
    char cod_ap_partida[MAX_COD_AP];
    char cod_ap_destino[MAX_COD_AP];
    short h_cheg_ap;
    short m_cheg_ap;
    scanf(" %s %s %hd:%hd", cod_ap_partida, cod_ap_destino, &h_cheg_ap, &m_cheg_ap);

    Cell *partida = hashtable_ap_get(hash_ap, fd_ap, cod_ap_partida);
    Cell *destino = hashtable_ap_get(hash_ap, fd_ap, cod_ap_destino);

    if (partida == NULL)
    {
        printf("+ aeroporto %s desconhecido\n", cod_ap_partida);
        return;
    }

    if (destino == NULL)
    {
        printf("+ aeroporto %s desconhecido\n", cod_ap_destino);
        return;
    }
    
    
    Caminho *caminho = dijkstra(hash_ap, hash_voo, fd_ap, fd_voo, partida, destino, h_cheg_ap, m_cheg_ap);
    
    if (caminho == NULL)
    {
        printf("+ sem voos de %s para %s\n", cod_ap_partida, cod_ap_destino);
        return;
    }
    printf("Voo    De   Para Parte Chega\n");
    printf("====== ==== ==== ===== =====\n");


    Node_caminho *atual = caminho->head->next;

    while (atual->next != NULL)
    {
        caminho_print(atual, false);
        atual = atual->next;
    }
    caminho_print(atual, true);

    printf("Tempo de viagem: %d minutos\n", caminho->destino->dist);

    caminho_destroy(caminho);
}

int main(void)
{
    FILE *fd_ap = open_file_ap();
    Ht_ap *hash_ap = hashtable_ap_new(fd_ap);
    
    FILE *fd_voo = open_file_voo();
    Ht_voo *hash_voo = hashtable_voo_new();
  //  hashtable_voo_read(hash_voo, fd_voo);

    char codigo[OP_SIZE];

    while (scanf(" %s", codigo) != EOF && strcmp(codigo, "X") != 0)
    {
        if (strcmp(codigo, "AI") == 0)
            introduzir_aeroporto(hash_ap, fd_ap);

        else if (strcmp(codigo, "FI") == 0)
            introduzir_voo(hash_voo, hash_ap, fd_voo, fd_ap);
        
        else if (strcmp(codigo, "FD") == 0)
            remover_voo(hash_voo, hash_ap, fd_voo, fd_ap);

        else if (strcmp(codigo, "TR") == 0)
            calcular_viagem(hash_voo, fd_voo, hash_ap, fd_ap);
    }

    hashtable_ap_write_and_destroy(hash_ap, fd_ap);
    hashtable_voo_write_and_destroy(hash_voo, fd_voo);
    
    fclose(fd_ap);
    fclose(fd_voo);

    return 0;
}