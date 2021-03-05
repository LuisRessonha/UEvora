#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "aeroportos.h"

Aeroporto *aeroporto_new(char cod_ap[MAX_COD_AP], short h_fuso, short m_fuso)
{
    Aeroporto *ap = malloc(sizeof(Aeroporto));

    strncpy(ap->cod_ap, cod_ap, MAX_COD_AP);
    ap->h_fuso_horario = h_fuso;
    ap->m_fuso_horario = m_fuso;
    ap->id = 0;

    return ap;
}

Aeroporto *aeroporto_new_empty()
{
    Aeroporto *ap = malloc(sizeof(Aeroporto));
    
    strncpy(ap->cod_ap, "", MAX_COD_AP);
    ap->h_fuso_horario = 0;
    ap->m_fuso_horario = 0;
    ap->id = -1;

    return ap;
}

void aeroporto_destroy(Aeroporto *ap)
{
    free(ap);
}

Voo *voo_new(char cod_voo[MAX_COD_VOO], char cod_ap_partida[MAX_COD_AP], char cod_ap_chegada[MAX_COD_AP], short h_partida, short m_partida, short duracao)
{
    Voo *voo = malloc(sizeof(Voo));

    strncpy(voo->cod_voo, cod_voo, MAX_COD_VOO);
    strncpy(voo->cod_ap_partida, cod_ap_partida, MAX_COD_AP);
    strncpy(voo->cod_ap_chegada, cod_ap_chegada, MAX_COD_AP);

    voo->h_partida = h_partida;
    voo->m_partida = m_partida;
    voo->duracao = duracao;
    voo->removido = 0;

    return voo;
}

Voo *voo_new_empty()
{
    Voo *voo = malloc(sizeof(Voo));
    
    strncpy(voo->cod_voo, "", MAX_COD_VOO);
    strncpy(voo->cod_ap_partida, "", MAX_COD_AP);
    strncpy(voo->cod_ap_chegada, "", MAX_COD_AP);

    voo->h_partida = 0;
    voo->m_partida = 0;
    voo->duracao = 0;
    voo->removido = 0;

    return voo;
}

void voo_destroy(Voo *voo)
{
    free(voo);
}