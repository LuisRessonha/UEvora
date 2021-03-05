#define _HEAP_H 1

#ifndef _AEROPORTOS_H
#include "aeroportos.h"
#endif

#ifndef _HTAP_H
#include "ht-aeroportos.h"
#endif

#ifndef _DIJKSTRA_H
#include "dijkstra.h"
#endif

typedef struct source {
    Cell *ap;
    int dist;  
    Cell *prev;
    Voo *voo;  
    short h_chegada;
    short m_chegada;
} Source;

typedef struct heap {
    int size;
    int count;
    int *posicoes;
    Source **heaparr;
} Heap;


Source *source_new();

void update_source(Source *velho, Source *novo);

Source *heapnode_new();

Heap *heap_new(int size);

void heap_destroy(Heap *heap);

void heap_display(Heap *h);

void swap_posicoes(int size, int pos[size], int id1, int id2);

void min_heapify(Heap *heap, int loc);

void heap_push(Heap *heap, Source *node);

Source *heap_pop(Heap *heap);