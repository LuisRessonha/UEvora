#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>



//------------ PCB + classes----------------

struct pcb {
	int begin;	//Início do processo
	int fim;	//Fim do processo
	int programcounter;		//Instrução em que se encontra
	int id;		//Identifica o processo
	int nBegin;	//Número de vezes que já fez um GOTOBEGIN10
	int qRun;	//verifica o quantum no RUN
	int qBlock;	//Verifica o numero de ciclos no block
	int estado;	//Guarda o estado atual
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

struct list *noavaFila();

void inserirPCB(struct pcb *p, struct list *noavaFila);

void printLista(struct list *noavaFila);

struct pcb * remover(int v, struct list *noavaFila); 

struct pcb * primeiroElemento(struct list *noavaFila);

bool vazio(struct list *noavaFila);

struct pcb *procurar(int v, struct list *noavaFila); 






// ------------------ Filas ------------------------------------------



// cria um node com o pcb

struct node *newNode(struct pcb *p, struct list *lista) 
{
	struct node *newNode = malloc(sizeof(struct node));

	newNode -> p = p;
	newNode -> next = NULL;

	return newNode;
}


//cria uma fila


struct list *noavaFila() 
{
	struct list *lista = malloc(sizeof(struct list));

	lista -> header = NULL;
	lista -> size = 0;

	return lista;
}


//insere o pcb no principio da lista


void inserirPCB(struct pcb *p, struct list *noavaFila) 
{
	struct node *node = newNode(p, noavaFila);

	if (noavaFila -> header == NULL) 
	{
		noavaFila -> header = node;
		noavaFila -> size = 1;
	} else {
		struct node *current = noavaFila -> header;

		while(current -> next != noavaFila -> header) 
		{
			current = current -> next;
		}
		current -> next = node;
		noavaFila -> size++;
	}

	node -> next = noavaFila -> header;
}

//print da lista

void printLista(struct list *noavaFila) {
	struct node * current = noavaFila -> header;

	for(int i = 0; i < noavaFila -> size; i++) 
	{
		printf("%d\n", current -> p -> id);
		current = current -> next;
	}
}

//devolve o header da lista -> PCB


struct pcb *primeiroElemento(struct list *noavaFila) 
{
	return noavaFila -> header -> p;
}


//recebe o número da lista e remove o ID da lista


struct pcb *remover(int v, struct list *noavaFila) 
{
	struct node *current = noavaFila ->header;
	struct pcb *safe;
	struct node *temp;

	if(noavaFila -> size > 1){		// before was != 0
			int i = 0;
			while(current -> next -> p -> id != v && i < noavaFila -> size + 1 ) 
			{
					current = current -> next;
					i++;
			}

			safe = current -> next -> p;
			temp = current -> next -> next;
			free(current -> next);
			current -> next = temp;
			noavaFila -> size--;
			noavaFila -> header = temp;

			return safe;

	} else if(noavaFila -> size == 1) 
	{
		noavaFila -> size--;
		safe = current -> p;
		free(current);
		noavaFila -> header = NULL;

		return safe;
	}

	return NULL;
}


//verifica se a lista está vazia ou não


bool vazio(struct list *noavaFila) 
{
	return noavaFila -> size == 0;
}

//Procura um certo id na fila noavaFila.
struct pcb *procurar(int v, struct list *noavaFila) 
{
	struct node *current = noavaFila ->header;
	struct pcb *safe;

	if(noavaFila -> size >= 1){
			int i = 0;
			while(current -> next -> p -> id != v && i < noavaFila -> size + 1 ) 
			{
				current = current -> next;
				i++;
			}
			safe = current -> next -> p;
			return safe;
	}
	return NULL;
}


