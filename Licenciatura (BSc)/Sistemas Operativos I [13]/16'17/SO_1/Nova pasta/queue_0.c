#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdbool.h>


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

struct list *newLista();

void insert(struct pcb *p, struct list *newLista);

void printLista(struct list *newLista);

struct pcb * remover(int v, struct list *newLista); 

struct pcb * top(struct list *newLista);

bool empty(struct list *newLista);

struct pcb *search(int v, struct list *newLista); 






// ------------------ Queue.c ----------------




struct node *newNode(struct pcb *p, struct list *lista) 
{
	struct node *newNode = malloc(sizeof(struct node));

	newNode -> p = p;
	newNode -> next = NULL;

	return newNode;
}

struct list *newLista() 
{
	struct list *lista = malloc(sizeof(struct list));

	lista -> header = NULL;
	lista -> size = 0;

	return lista;
}

void insert(struct pcb *p, struct list *newLista) 
{
	struct node *node = newNode(p, newLista);

	if (newLista -> header == NULL) 
	{
		newLista -> header = node;
		newLista -> size = 1;
	} else {
		struct node *current = newLista -> header;

		while(current -> next != newLista -> header) 
		{
			current = current -> next;
		}
		current -> next = node;
		newLista -> size++;
	}

	node -> next = newLista -> header;
}

void printLista(struct list *newLista) {
	struct node * current = newLista -> header;

	for(int i = 0; i < newLista -> size; i++) 
	{
		printf("%d\n", current -> p -> id);
		current = current -> next;
	}
}

struct pcb *top(struct list *newLista) 
{
	return newLista -> header -> p;
}

struct pcb *remover(int v, struct list *newLista) 
{
	struct node *current = newLista ->header;
	struct pcb *safe;
	struct node *temp;

	if(newLista -> size > 1){		// before was != 0
			int i = 0;
			while(current -> next -> p -> id != v && i < newLista -> size + 1 ) 
			{
					current = current -> next;
					i++;
			}

			safe = current -> next -> p;
			temp = current -> next -> next;
			free(current -> next);
			current -> next = temp;
			newLista -> size--;
			newLista -> header = temp;

			return safe;

	} else if(newLista -> size == 1) 
	{
		newLista -> size--;
		safe = current -> p;
		free(current);
		newLista -> header = NULL;

		return safe;
	}

	return NULL;
}

bool empty(struct list *newLista) 
{
	return newLista -> size == 0;
}

//Procura um certo id na queue newLista.
struct pcb *search(int v, struct list *newLista) 
{
	struct node *current = newLista ->header;
	struct pcb *safe;

	if(newLista -> size >= 1){
			int i = 0;
			while(current -> next -> p -> id != v && i < newLista -> size + 1 ) 
			{
				current = current -> next;
				i++;
			}
			safe = current -> next -> p;
			return safe;
	}
	return NULL;
}