#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define NOME_FICHEIRO "quartos.bin"

typedef struct quarto {
    char edificio;
    int piso;
    int numero;   
} Quarto;

Quarto *new_quarto()
{
    Quarto *q = malloc(sizeof(Quarto));

    q->edificio = ' ';
    q->numero = 0;
    q->piso = 0;

    return q;
}

void guardar_quarto(Quarto q, FILE *fd)
{
    int n_quartos;
    rewind(fd);

    if (fread(&n_quartos, sizeof(int), 1, fd) != 1)
        n_quartos = 0;
    else
        n_quartos++;
  
    fseek(fd, 0, SEEK_END);
    fwrite(&q, sizeof(Quarto), 1, fd);

    rewind(fd);
    fwrite(&n_quartos, sizeof(n_quartos), 1 , fd);
}

void ler_quartos(FILE *fd)
{   
    int n_quartos;
    rewind(fd);
    fread(&n_quartos, sizeof(n_quartos), 1, fd);
    printf("Nº de quartos: %d\n", n_quartos+1);

    for (int i = 0; i <= n_quartos; i++)
    {
        Quarto q;
        fread(&q, sizeof(Quarto), 1, fd);
        printf("Edificio: %c\nPiso:     %d\nNumero:   %d\n-------------\n", q.edificio, q.piso, q.numero);
    }
}

int main()
{
    char ed = 'A';
    int piso, num;
    FILE *fd = fopen(NOME_FICHEIRO, "w+");


    while (ed != 'X')
    {

        scanf(" %c", &ed);

        switch(ed)
        {
            case 'X':
                break;

            case 'L':
                ler_quartos(fd);
                break;

            default:
                scanf(" %d %d", &piso, &num);
                Quarto q;
                q.edificio = ed;
                q.piso = piso;
                q.numero = num;
                guardar_quarto(q, fd);
        }      
    }

    fclose(fd);

}