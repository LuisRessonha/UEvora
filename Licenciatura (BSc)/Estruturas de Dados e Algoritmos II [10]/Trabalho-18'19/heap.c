#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

#include "heap.h"


Source *source_new()
{
    Source *s = malloc(sizeof(Source));

    s->ap = NULL;
    s->dist = INT_MAX;
    s->prev = NULL;
    s->voo = NULL;
    s->h_chegada = -1;
    s->m_chegada = -1;

    return s;
}

void update_source(Source *velho, Source *novo)
{
    velho->ap = novo->ap;
    velho->dist = novo->dist;
    velho->prev = novo->prev;
    velho->voo = novo->voo;
    velho->h_chegada = novo->h_chegada;
    velho->m_chegada = novo->m_chegada;
}

Heap *heap_new(int size)
{
    Heap *heap = malloc(sizeof(Heap));

    heap->size = size;
    heap->count = 0;
    heap->posicoes =  malloc(sizeof(int) * size);

    for (int i = 0; i < size; i++)
        heap->posicoes[i] = -1;

    heap->heaparr = calloc(size, sizeof(Source));

    return heap;
}

void heap_destroy(Heap *heap)
{
    if (heap->posicoes != NULL)
        free(heap->posicoes);

    if (heap->heaparr != NULL)
        free(heap->heaparr);
    
    free(heap);
}

void heap_display(Heap *h) {
	int i;
   // printf("Array da Heap: \n");
    printf("\n");

	for (i = 0; i < h->count; ++i)
	{
        printf("Pos: %-2d --- Array Pos: %-4d | ", i, h->posicoes[h->heaparr[i]->ap->ap->id]);
    }
    printf("\n");

    for (i = 0; i < h->count; ++i)
    {
		printf("Ap ID: %-20d | ", h->heaparr[i]->ap->ap->id);
    }
	printf("\n");

    for (i = 0; i < h->count; ++i)
    {
		printf("Cod Voo: %-17s  | ", h->heaparr[i]->voo->cod_voo);
    }
	printf("\n");

    for (i = 0; i < h->count; ++i)
    {
		printf("Dist: %-19d   | ", h->heaparr[i]->dist);
    }
	printf("\n");


    for (i = 0; i < h->count; ++i)
    {
        if(h->heaparr[i]->voo != NULL)
            printf("Hora Partida: %2d:%-10.2d | ", h->heaparr[i]->voo->h_partida, h->heaparr[i]->voo->m_partida);
    }   
    printf("\n");

    for (i = 0; i < h->count; ++i)
    {
        if(h->heaparr[i]->voo != NULL)
            printf("Ap Partida: %-15s | ", h->heaparr[i]->voo->cod_ap_partida);
    }   
    printf("\n");


    for (i = 0; i < h->count; ++i)
    {
        if(h->heaparr[i]->voo != NULL)
            printf("Ap Destino: %-15s | ", h->heaparr[i]->voo->cod_ap_chegada);
    }   
    printf("\n");
}



void swap_posicoes(int size, int pos[size], int id1, int id2)
{
    int temp = pos[id1];
    pos[id1] = pos[id2];
    pos[id2] = temp; 
}

void swap_nodes(Heap *heap, int pos1, int pos2)
{
    Source *temp = heap->heaparr[pos1];
    heap->heaparr[pos1] = heap->heaparr[pos2];
    heap->heaparr[pos2] = temp;
    swap_posicoes(heap->count, heap->posicoes, heap->heaparr[pos1]->ap->ap->id, heap->heaparr[pos2]->ap->ap->id);
}

void min_heapify_down(Heap *heap, int current)
{
    int left = (2*current) + 1;
    int right = left + 1;

    if (left < heap->count && right < heap->count 
        &&(heap->heaparr[current]->dist > heap->heaparr[left]->dist || heap->heaparr[current]->dist > heap->heaparr[right]->dist))
    {
        if (heap->heaparr[left]->dist < heap->heaparr[right]->dist)
        {
            swap_nodes(heap, current, left);
            min_heapify_down(heap, left);
        }
        else
        {
            swap_nodes(heap, current, right);
            min_heapify_down(heap, right);
        }
    }
}

void min_heapify_up(Heap *heap, int current)
{
    int parent = (current-1)/2;

    if (parent >= 0 && heap->heaparr[parent]->dist > heap->heaparr[current]->dist)
    {
        swap_nodes(heap, parent, current);
        min_heapify_up(heap, parent);
    }
}


bool heap_exists(Heap *heap, int id)
{
    return heap->posicoes[id] != -1;
}

void heap_update(Heap *heap, Source *novo)
{
    //heap->heaparr[heap->posicoes[novo->ap->ap->id]]->dist = novo->dist;
    
    update_source(heap->heaparr[heap->posicoes[novo->ap->ap->id]], novo);
    
    min_heapify_up(heap, heap->posicoes[novo->ap->ap->id]);
    free(novo);
}

void heap_push(Heap *heap, Source *s)
{   
    if (heap_exists(heap, s->ap->ap->id))
    {
        heap_update(heap, s);
        return;
    }
    heap->heaparr[heap->count]= s;
    heap->posicoes[s->ap->ap->id] = heap->count;
    int pos = heap->count;

    heap->count++;
    min_heapify_up(heap, pos);
   
}

Source *heap_pop(Heap *heap)
{
    min_heapify_down(heap, 0);

    min_heapify_up(heap, heap->count-1);

// //////////////////////////////////////////////////////////////
//     printf("Heap Antes do pop: \n");                    //////
//     heap_display(heap);                                 //////
//     printf("\n///////////////////////////////\n\n\n");  //////
// //////////////////////////////////////////////////////////////


    heap->count--;
    swap_nodes(heap, 0, heap->count);
    Source *popped = heap->heaparr[heap->count];
    
    min_heapify_down(heap, 0);

// //////////////////////////////////////////////////////////////
//     printf("Heap depois do pop: \n");                   //////
//     heap_display(heap);                                 //////
//     printf("\n///////////////////////////////\n\n\n");  //////
// //////////////////////////////////////////////////////////////


    return popped;

}