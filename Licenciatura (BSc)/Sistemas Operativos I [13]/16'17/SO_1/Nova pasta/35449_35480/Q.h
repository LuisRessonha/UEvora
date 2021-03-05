#include <stdio.h>

struct pcb {
	int ini;	//Início do processo.
	int fim;	//Fim do processo.
	int pc;		//Instrução em que se encontra.
	int id;		//Identificador único.
	int nBegin;	//Número de vezes que já fez um GOTOBEGIN10.
	int qRun;	//verifica o quantum no RUN.
	int qBlock;	//Verifica o numero de ciclos no block.
	int estado;	//Guarda o estado em que está atualmente.
};

struct node {
	struct pcb *p;
	struct node *next;
};

struct list {
	struct node *header;
	int size;
};

struct node *newNode(struct pcb *p, struct list *lista);

struct list *newLista();

void insert(struct pcb *p, struct list *newLista);

void printLista(struct list *newLista);

struct pcb * remover(int v, struct list *newLista); 

struct pcb * top(struct list *newLista);

bool empty(struct list *newLista);

struct pcb *search(int v, struct list *newLista); 