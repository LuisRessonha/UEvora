#define _AEROPORTOS_H 1

#include <stdbool.h>

#define MAX_COD_AP 5
#define MAX_COD_VOO 7


typedef struct aeroporto {
    char cod_ap[MAX_COD_AP];    // 3 ou 4 chars
    int id;                     // id unico para cada aeroporto para o djikstra
    short h_fuso_horario;       // entre -12 e 14
    short m_fuso_horario;       // entre 0 e 59 
} Aeroporto;    

typedef struct voo {
    char cod_voo[MAX_COD_VOO];   // 2 chars seguidos por 1 a 4 algarismos
    char cod_ap_partida[MAX_COD_AP];         
    char cod_ap_chegada[MAX_COD_AP];
    short h_partida;            // entre 0 e 23
    short m_partida;            // entre 0 e 59
    short duracao;              // entre 0 e 1440 
    short removido;
    
} Voo; 

Aeroporto *aeroporto_new_empty();
Aeroporto *aeroporto_new(char cod_ap[MAX_COD_AP], short h_fuso, short m_fuso);
void aeroporto_destroy(Aeroporto *ap);
Voo *voo_new(char cod_voo[MAX_COD_VOO], char cod_ap_partida[MAX_COD_AP], char cod_chegada[MAX_COD_AP], short h_partida, short m_partida, short duracao);
Voo *voo_new_empty();
void voo_destroy(Voo *voo);