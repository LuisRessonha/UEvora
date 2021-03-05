#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "fila.h"


#define MAXPROCESS 6
#define MAXREADY 4
#define QUANTUM 3
#define ARRAYMAXSIZE 100

struct list *new;
struct list *ready;
struct list *running;
struct list *block;
struct list *sair1;
struct list *ordem;

int nProc = -1;
int nReady = 0;
int procId = -1;
int tempo = 0;

char arrayP[51];
char arrayI[50];

struct pcb *novoControlBlock(int procId) 
{
	struct pcb *novoControlBlock = malloc(sizeof(struct pcb));

	novoControlBlock -> begin = 0;
	novoControlBlock -> fim = 0;
	novoControlBlock -> programcounter = 0;
	novoControlBlock -> id = procId;
	novoControlBlock -> nBegin = 0;
	novoControlBlock -> qRun = 0;
	novoControlBlock -> qBlock = 0;
	novoControlBlock -> estado = -1;
	
	return novoControlBlock;
}

void estadoNewParaReady() 
{
	inserirPCB(remover(primeiroElemento(new) -> id, new),ready);
}

void estadoBlockParaReady() 
{
	inserirPCB(remover(primeiroElemento(block) -> id, block), ready);
}

void estadoReadyParaRun() 
{
	inserirPCB(remover(primeiroElemento(ready) -> id, ready), running);
}

void estadoRunParaReady() 
{
	inserirPCB(remover(primeiroElemento(running) -> id, running), ready);
}

void estadoRunParaBlock() 
{
	inserirPCB(remover(primeiroElemento(running) -> id, running), block);
}

void estadoRunParaExit() 
{
	inserirPCB(remover(primeiroElemento(running) -> id, running), sair1);
}




// Verificações 
void verificacoes() 
{
	tempo++;
	//Verifica o Run.
	if (!vazio(running))
	{ //Verificação da Queue Run.

		struct pcb *pcb = primeiroElemento(running);

		if (pcb -> qRun < QUANTUM) 
		{
			if (arrayP[pcb -> programcounter] == '5') 
			{
				pcb -> programcounter++;
			}
			//faz sair1
			else if (arrayP[pcb -> programcounter] == '0') 
			{
				nProc--;
				for (int i = pcb -> begin; i < pcb -> fim; i++)
				{
					arrayP[i] = 'x';
				}
				pcb -> estado = 4;
				estadoRunParaExit();
			}
			//Faz CPU
			else if (arrayP[pcb -> programcounter] == '1')
			{
				pcb -> programcounter++;
				pcb -> qRun++;
			}
			//Faz Block
			else if (arrayP[pcb -> programcounter] == '2') 
			{
				pcb -> qRun = 0;
				pcb -> estado = 3;
				estadoRunParaBlock();
			}
			//Faz GoToBegin
			else if (arrayP[pcb -> programcounter] == '3') 
			{
				if (pcb -> nBegin < 8) 
				{
					pcb -> programcounter = pcb -> begin;
					pcb -> nBegin++;
					pcb -> qRun++;
					nReady++;
				}
				else 
				{
					arrayP[pcb -> programcounter] = '5';
					pcb -> nBegin = 0;
					pcb -> programcounter = pcb -> begin;
					pcb -> qRun = 0;
					nReady++;
					pcb -> estado = 1;
					estadoRunParaReady();
				}
			}

			
		else 
		{
			pcb -> qRun = 0;
			nReady++;
			pcb -> estado = 1;
			estadoRunParaReady();
		}
	}

	// Verificar Block.
	if (!vazio(block))
	{
		struct pcb *pcb = primeiroElemento(block);

		if (pcb -> qBlock < 3) 
		{
			pcb -> qBlock++;
		}
		else 
		{
			pcb -> estado = 1;
			pcb -> qBlock = 0;
			nReady++;
			pcb -> programcounter++;
			estadoBlockParaReady();
		}
	}

	//Verifica se pode ir de ready para Run.
	if(vazio(running) && !vazio(ready)) 
	{
		struct pcb * pcb = primeiroElemento(ready);
		nReady--;
		pcb -> estado = 2;
		estadoReadyParaRun();
	}

	//Verifica de pode ir de New para Ready.
	if (nReady < MAXREADY && !vazio(new)) 
	{
		struct pcb *pcb = primeiroElemento(new);
		nReady++;
		pcb -> estado = 1;
		estadoNewParaReady();
	}
}
}	


void writeInFile(FILE *nfp, struct list *ordem, int tempo) 
{

	fprintf(nfp, "%d |", tempo);

	for(int j = 0; j <= procId; j++) 
	{
		struct pcb *temp = procurar(j, ordem);
		int i = temp -> estado;

		if (i == 0)
		{
			fprintf(nfp, " NEW |");
		}else if(i == 1) 
		{
			fprintf(nfp, " READY |");
		}else if(i == 2) 
		{
			fprintf(nfp, " RUN |");
		}else if (i == 3) 
		{
			fprintf(nfp, " BLOCK |");
		}else if (i == 4) 
		{
			fprintf(nfp, " sair1 |");
		}
	}
	fprintf(nfp, "\nMEM | ");
	for (int i = 0; i < 50; i++)
	{
		fprintf(nfp, "%c", arrayP[i]);
	}
	fprintf(nfp, "\n--------------------------------------------------------------------------------\n");
}


//------------------------MAIN----------------------


//----------- Abir ficheiro -----------
int openFile(){

	char str[150];
	scanf("%s", str);

	FILE * fPointer;
	fPointer = fopen(str, "r");
	size_t len = 0;

	char *asdf = NULL;
	getline(&asdf, &len, fPointer);

	int arrayQwerty[ARRAYMAXSIZE];

	int i = 0;


	const char s[2] = " ";
	char *token;


	token = strtok(asdf, s);
	arrayQwerty[i] = atoi(token);


	while( token != NULL ) {
	  printf( " %s\n", token );

	  token = strtok(NULL, s);
	  arrayQwerty[i] = atoi(token);
	  i++;
	}

	for (int i = 0; i < ARRAYMAXSIZE; i++)
		printf("%d\n", arrayQwerty[i]);

	fclose(fPointer);

	return(0);
}


//------------------------------------



int main(void){

	openFile(); //Abrir o ficheiro


	new = noavaFila();
	ready = noavaFila();
	running = noavaFila();
	block = noavaFila();
	sair1 = noavaFila();
	ordem = noavaFila();

	}