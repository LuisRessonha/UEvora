#define _DIJKSTRA_H 1

#ifndef _AEROPORTOS_H
#include "aeroportos.h"
#endif

#ifndef _HTAP_H
#include "ht-aeroportos.h"
#endif

#ifndef _HTVOO_H
#include "ht-voos.h"
#endif

#ifndef _HEAP_H
#include "heap.h"
#endif

typedef struct node_caminho {
    char cod_ap_partida[MAX_COD_AP];
    char cod_ap_chegada[MAX_COD_AP];
    Voo *voo;
    int dist;
    short h_chegada;
    short m_chegada;
    struct node_caminho *next;
} Node_caminho;

typedef struct caminho {
    Node_caminho *head;
    Node_caminho *destino;
} Caminho;

Caminho *dijkstra(Ht_ap *hash_ap, Ht_voo *hash_voo, FILE* fd_ap, FILE * fd_voo, Cell *partida, Cell *destino_final, short h_inicio, short m_inicio);
void caminho_print(Node_caminho *node , bool ultimo);
void caminho_destroy(Caminho *cam);

