#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "queue.c"

#define MAXPROCESS 6
#define MAXREADY 4
#define QUANTUM 3

struct list *new;
struct list *ready;
struct list *running;
struct list *block;
struct list *finished;
struct list *ordem;

int nProc = -1;
int nReady = 0;
int procId = -1;
int tempo = 0;

char arrayP[51];
char arrayI[50];

struct pcb *newPcb(int procId) 
{
	struct pcb *newPcb = malloc(sizeof(struct pcb));

	newPcb -> begin = 0;
	newPcb -> fim = 0;
	newPcb -> programcounter = 0;
	newPcb -> id = procId;
	newPcb -> nBegin = 0;
	newPcb -> qRun = 0;
	newPcb -> qBlock = 0;
	newPcb -> estado = -1;
	
	return newPcb;
}

void newToReady() 
{
	insert(remover(top(new) -> id, new),ready);
}

void blockToReady() 
{
	insert(remover(top(block) -> id, block), ready);
}

void readyToRun() 
{
	insert(remover(top(ready) -> id, ready), running);
}

void runToReady() 
{
	insert(remover(top(running) -> id, running), ready);
}

void runToBlock() 
{
	insert(remover(top(running) -> id, running), block);
}

void runToFinished() 
{
	insert(remover(top(running) -> id, running), finished);
}




// Realiza todas as verificações e trocas do scheduler.
void ciclo() 
{
	tempo++;
	//Verifica o Run.
	if (!empty(running))
	{ //Verificação da Queue Run.

		struct pcb *pcb = top(running);

		if (pcb -> qRun < QUANTUM) 
		{
			if (arrayP[pcb -> programcounter] == '5') 
			{
				pcb -> programcounter++;
			}
			//faz Exit
			else if (arrayP[pcb -> programcounter] == '0') 
			{
				nProc--;
				for (int i = pcb -> begin; i < pcb -> fim; i++)
				{
					arrayP[i] = 'x';
				}
				pcb -> estado = 4;
				runToFinished();
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
				runToBlock();
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
					runToReady();
				}
			}

			//Ignora o Fork
			else if (arrayP[pcb -> programcounter] == '4') 
			{
				arrayP[pcb -> programcounter] = '5';
			}

		}
		else 
		{
			pcb -> qRun = 0;
			nReady++;
			pcb -> estado = 1;
			runToReady();
		}
	}

	// Verificar Block.
	if (!empty(block))
	{
		struct pcb *pcb = top(block);

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
			blockToReady();
		}
	}

	//Verifica se pode ir de ready para Run.
	if(empty(running) && !empty(ready)) 
	{
		struct pcb * pcb = top(ready);
		nReady--;
		pcb -> estado = 2;
		readyToRun();
	}

	//Verifica de pode ir de New para Ready.
	if (nReady < MAXREADY && !empty(new)) 
	{
		struct pcb *pcb = top(new);
		nReady++;
		pcb -> estado = 1;
		newToReady();
	}
}	


void writeInFile(FILE *nfp, struct list *ordem, int tempo) 
{

	fprintf(nfp, "%d |", tempo);

	for(int j = 0; j <= procId; j++) 
	{
		struct pcb *temp = search(j, ordem);
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
			fprintf(nfp, " EXIT |");
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
		printf("%s\n", arrayQwerty[i]);

	fclose(fPointer);

	return(0);
}


//------------------------------------



int main(void){

	openFile() //Abrir o ficheiro


	new = newLista();
	ready = newLista();
	running = newLista();
	block = newLista();
	finished = newLista();
	ordem = newLista();


}