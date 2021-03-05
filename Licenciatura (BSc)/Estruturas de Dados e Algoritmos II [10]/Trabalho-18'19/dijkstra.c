#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "dijkstra.h"

#define ESPERA_ESCALA 30
#define MIN_NUM_DIA 1440

Node_caminho *node_c_new(Source *s)
{
    Node_caminho *node = malloc(sizeof(Node_caminho));

    if (s->prev != NULL)
        strncpy(node->cod_ap_partida, s->prev->ap->cod_ap, MAX_COD_AP);
    strncpy(node->cod_ap_chegada, s->ap->ap->cod_ap, MAX_COD_AP);
    node->voo = s->voo;
    node->dist = s->dist;
    node->h_chegada = s->h_chegada;
    node->m_chegada = s->m_chegada;

    node->next = NULL;

    return node;
}

Caminho *caminho_new(Source *destino)
{
    Caminho *cam = malloc(sizeof(Caminho));

    cam->destino = node_c_new(destino);
    cam->head = cam->destino;

    return cam;
}

void node_c_destroy(Node_caminho *node)
{
    if (node->next != NULL)
        node_c_destroy(node->next);
    
    free(node);     // SE DER MERDA TAMBEM É AQUI
}

void caminho_destroy(Caminho *cam)
{
    if (cam->head != NULL)
        node_c_destroy(cam->head);
    
    free(cam);
}

void caminho_insert(Caminho *c, Source *s)
{
    Node_caminho *novo = node_c_new(s);
    novo->next = c->head;
    c->head = novo;
}

int clock_to_dur(short horas, short min)
{
    int dur = 0;
    dur += horas * 60;

    if (horas < 0)
        dur -= min;
    else
        dur += min;

    return dur;
}

void dur_to_clock(short clock[2], int dur)
{
    
    clock[0] = dur / 60;
    clock[1] = dur % 60;

    while (clock[0] > 23 || clock[1] > 59)
    {
        if (clock[0] > 23)
        {
            clock[1] += (clock[0] % 24) * 60;
            clock[0] = clock[0] % 24;
        }

        if (clock[1] > 59)
        {
            clock[0] += clock[1] / 60;
            clock[1] = clock[1] % 60;
        } 
    }
}

void calculo_chegada(short clock[2], short hp, short mp, short fh_hp, short fh_mp, short dur, short fh_hd, short fh_md)
{
    short min_partida = clock_to_dur(hp, mp);
    short min_fh_partida = clock_to_dur(fh_hp, fh_mp);
    short min_fh_destino = clock_to_dur(fh_hd, fh_md);
    int min_destino;

    if ( (min_fh_destino < 0 && min_fh_partida < 0) || (min_fh_destino > 0 && min_fh_partida > 0))
        min_destino = min_partida + (min_fh_destino - min_fh_partida) + dur;      
    else
        min_destino = min_partida + min_fh_destino + (-min_fh_partida) + dur;

    while (min_destino > MIN_NUM_DIA)
        min_destino -= MIN_NUM_DIA;
        
    while (min_destino < 0)
        min_destino += MIN_NUM_DIA;


    dur_to_clock(clock, min_destino);
}

void relax(Heap *heap, int n,  Source *todos[n], Source *velho, Source *novo)
{
    if (novo->dist <= velho->dist)
    {   
        // update_source(todos[velho->ap->ap->id], novo);
        heap_push(heap, novo);
    }
    else
    {
        free(novo);
    }
    
}

void caminho_print(Node_caminho *node , bool ultimo)
{
    int chegada = clock_to_dur(node->h_chegada, node->m_chegada);
    short clock[2];
    dur_to_clock(clock, chegada);

    printf("%-6s %-4s %-4s %02hd:%02hd %02hd:%02hd\n", 
    node->voo->cod_voo, node->cod_ap_partida, node->cod_ap_chegada, node->voo->h_partida, node->voo->m_partida, node->h_chegada, node->m_chegada);
}

int tempo_de_espera(short h_i, short m_i, short h_v, short m_v, bool intermedio)
{
    int inicial = clock_to_dur(h_i, m_i);
    int final = clock_to_dur(h_v, m_v);

    if (final >= inicial)
    {
        if (final-inicial < ESPERA_ESCALA && intermedio)
            return MIN_NUM_DIA + (final - inicial);

         return final - inicial;
    }
    else
        return MIN_NUM_DIA + (final - inicial);
}


Caminho *dijkstra(Ht_ap *hash_ap, Ht_voo *hash_voo, FILE* fd_ap, FILE * fd_voo, Cell *partida, Cell *destino_final, short h_inicio, short m_inicio)
{
    // inicializacoes
    Heap *heap = heap_new(hash_ap->n_ap);
    Source *todos[hash_ap->n_ap];
    bool visitados[hash_ap->n_ap];
    for (int i = 0; i < hash_ap->n_ap; i++)
    {
        visitados[i] = false;
        todos[i] = NULL;
    }
    short clock[2];
    Voo *v;
    Cell *destino;
    Node *voo_atual;

    // aerporto de partida
    Source *primeiro = source_new();
    primeiro->ap = partida;
    primeiro->dist = 0;
    heap_push(heap, primeiro);
    todos[partida->ap->id] = primeiro;
    visitados[partida->ap->id] = true;
    Source *poped = primeiro;
    bool first = true;

    // DIJKSTRA
    while (heap->count > 0)
    {
        poped = heap_pop(heap);
        visitados[poped->ap->ap->id] = true;

        if (poped->ap->ap->id == destino_final->ap->id)
            break;

        voo_atual = poped->ap->voos->head->next;
       //  printf("ap de partida: %s\n", poped->ap->ap->cod_ap);
       //  printf("nº de voos: %d!\n", poped->ap->voos->length);
        while (voo_atual != NULL)
        {
         //   printf("cod_voo: %s!\n", voo_atual->cod_voo);
            v = hashtable_voo_get(hash_voo, fd_voo, voo_atual->cod_voo);
            if (v == NULL)
                break;
            destino = hashtable_ap_get(hash_ap, fd_ap, v->cod_ap_chegada);

            if (!visitados[destino->ap->id])
            {
                calculo_chegada(clock, v->h_partida, v->m_partida, poped->ap->ap->h_fuso_horario, poped->ap->ap->m_fuso_horario, v->duracao, destino->ap->h_fuso_horario, destino->ap->m_fuso_horario);
                Source *novo = source_new();
                novo->ap = destino;
                if (first)
                    novo->dist = v->duracao + tempo_de_espera(h_inicio, m_inicio, v->h_partida, v->m_partida, false);
                else
                {
                    int espera = tempo_de_espera(poped->h_chegada, poped->m_chegada, v->h_partida, v->m_partida, true);
                    novo->dist = poped->dist + v->duracao + espera;
                }
                
                novo->prev = poped->ap;
                novo->voo = v;
                novo->h_chegada = clock[0];
                novo->m_chegada = clock[1];

                if (todos[novo->ap->ap->id] == NULL)
                    todos[novo->ap->ap->id] = novo;

                relax(heap, hash_ap->n_ap, todos, todos[novo->ap->ap->id], novo);
            }

            voo_atual = voo_atual->next;
        }

        first = false;
    }

    // Retornar caminho
    if (todos[destino_final->ap->id] == NULL)
    {
        for (int i = 0; i < hash_ap->n_ap; i++)
            free(todos[i]);

        heap_destroy(heap);
        return NULL;
    }

    Source *node = todos[destino_final->ap->id];
    Caminho *caminho = caminho_new(node);

    while (node->prev != NULL)
    {
        caminho_insert(caminho, todos[node->prev->ap->id]);
        node = todos[node->prev->ap->id];
    }

    heap_destroy(heap);
    
    
    for (int i = 0; i < hash_ap->n_ap; i++)
        free(todos[i]);

   

    return caminho;
}