#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "Q.h"

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

	newPcb -> ini = 0;
	newPcb -> fim = 0;
	newPcb -> pc = 0;
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
			if (arrayP[pcb -> pc] == '5') 
			{
				pcb -> pc++;
			}
			//faz Exit
			else if (arrayP[pcb -> pc] == '0') 
			{
				nProc--;
				for (int i = pcb -> ini; i < pcb -> fim; i++)
				{
					arrayP[i] = 'x';
				}
				pcb -> estado = 4;
				runToFinished();
			}
			//Faz CPU
			else if (arrayP[pcb -> pc] == '1')
			{
				pcb -> pc++;
				pcb -> qRun++;
			}
			//Faz Block
			else if (arrayP[pcb -> pc] == '2') 
			{
				pcb -> qRun = 0;
				pcb -> estado = 3;
				runToBlock();
			}
			//Faz GoToBegin
			else if (arrayP[pcb -> pc] == '3') 
			{
				if (pcb -> nBegin < 8) 
				{
					pcb -> pc = pcb -> ini;
					pcb -> nBegin++;
					pcb -> qRun++;
					nReady++;
				}
				else 
				{
					arrayP[pcb -> pc] = '5';
					pcb -> nBegin = 0;
					pcb -> pc = pcb -> ini;
					pcb -> qRun = 0;
					nReady++;
					pcb -> estado = 1;
					runToReady();
				}
			}

			//Ignora o Fork
			else if (arrayP[pcb -> pc] == '4') 
			{
				arrayP[pcb -> pc] = '5';
			}

			/*
			//Faz Fork
			else if (arrayP[pcb -> pc] == 4) 
			{
				if (nProc < MAXPROCESS-1)
				{
					procId++;
					struct pcb *novo = newPcb(procId);

					for (int i = 0; i < 101; i++)
					{
						novo -> command[i] = pcb -> command[i];
					}
					novo -> estado = 1;
					novo -> pc = 0;
					pcb -> pc++;
					pcb -> qRun++;
					nProc++;
					insert(novo, ready);
					insert(novo, ordem);
				}
				else 
				{
					pcb -> pc ++;
				}
			}*/

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
			pcb -> pc++;
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

	if (nfp == NULL) 
	{
		printf("Ficheiro inexistente!\n");
	}

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

int main(void)
{
	new = newLista();
	ready = newLista();
	running = newLista();
	block = newLista();
	finished = newLista();
	ordem = newLista();


	for(int p = 0; p < 50; p++) 
	{
		arrayP[p] = 'x';
	}

	arrayP[51] = 'A';
	bool escreve = true;
	char k[10] ;
	char fileRead[30];
	FILE *fp;
	FILE *nfp = fopen("scheduler.out", "a");

	while(escreve) 
	{
		printf("worst ou best?\n");
		scanf("%s", k);

		//WORST FIT
		if (strcmp(k, "worst") == 0)
		{
			while (true) 
			{

				printf("Introduzir nome do ficheiro, ou caso queira sair, exit: \n");
				scanf("%s", fileRead);

				if(strcmp(fileRead, "exit") != 0)
				{
					if (nProc < MAXPROCESS)
					{
						ciclo();
						fp = fopen(fileRead, "r");
						if(fp == NULL) 
						{
							printf("O ficheiro não existe.\n");
						} 
						else 
						{
							char c;
							int posInt = 0;	//sitio onde introduzir a instrução.
							int nIns = 0;	//número de instruções.
							while(true) 
							{
								c = fgetc(fp);
								if(feof(fp)) 
								{
									break;
								}
								if(c != ' ')
								{ //Escrever no array inicial.
									arrayI[posInt] = c;
									posInt++;
									nIns++;
								}
							}

							int q = 0;
							int t = 0;
							int pos = 0; //posição final

							for (int i = 0; i <=50 ; ++i)
							{
								char h = arrayP[i];
								if (h == 'x')
								{
									q++;
								}
								else 
								{
									if (q > t)
									{
										t = q;;
										pos = i - 1;
										q = 0;
									}
									else
									{
										q = 0;
									}
								}
							}
							if(t < nIns) 
							{
								printf("Memória cheia - Impossível criar processo.\n");
								writeInFile(nfp, ordem, tempo);
								ciclo();
							}
							else 
							{ //Caso tenha memória disponível:
								procId++;
								struct pcb *x = newPcb(procId);

								//alterar valores.
								int inicio = pos - t + 1;
								x -> fim = inicio + nIns;
								x -> ini = inicio;
								x -> pc = inicio;
								x -> estado = 0;

								int w =0;
								for (int g = x -> ini; g <= x -> fim - 1; g++)
								{
									if (arrayI[w] != '0' )
									{
										arrayP[g] = arrayI[w];
										w++;	
									}
									else 
									{
										arrayP[g] = '0';  //arrayI[w];
									}
								}//fim de encher arrayP.
								//inserir nas Qs.
								nProc++;
								insert(x, new);
								insert(x, ordem);
								writeInFile(nfp, ordem, tempo);
								fclose(fp);
							}
						}//fim da criação de novo processo.
					}
					else //número de processos igual ao máximo.
					{
						printf("Número máximo de ficheiros atingido.\n");
						writeInFile(nfp, ordem, tempo);
						ciclo();
					}
				}//fim de introduzir ficheiros.
				else //Até não haver ficheiros
				{
					while(nProc != -1) 
					{
						writeInFile(nfp, ordem, tempo);
						ciclo();
					}

					writeInFile(nfp, ordem, tempo);
					escreve = false;
					break;	
				}
			}
		}//fim worst fit

		//BEST FIT
		if (strcmp(k, "best") == 0)
		{
			while (true) 
			{

				printf("Introduzir nome do ficheiro, ou caso queira sair, exit: \n");
				scanf("%s", fileRead);

				if(strcmp(fileRead, "exit") != 0) 
				{
					if (nProc < MAXPROCESS)
					{
						ciclo();
						fp = fopen(fileRead, "r");
						if(fp == NULL) 
						{
							printf("O ficheiro não existe.\n");
						} 
						else 
						{
							char c;
							int posInt = 0;	//sitio onde introduzir a instrução.
							int nIns = 0;	//número de instruções.
							while(true) 
							{
								c = fgetc(fp);
								if(feof(fp)) 
								{
									break;
								}
								if(c != ' ')
								{ //Escrever no array inicial.
									arrayI[posInt] = c;
									posInt++;
									nIns++;
								}
							}

							int q = 0;
							int t = 50;
							int pos = 0; //posição no array final

							for (int i = 0; i <= 50; ++i)
							{
								char h = arrayP[i];

								if (h == 'x')
								{
									q++;
								}
								else 
								{
									if (q <= t && q >= nIns)
									{
										t = q;
										pos = i - 1;
									}
									else
									{
										q = 0;
									}
								}
							}

							if(q < nIns) //Memória indisponível.
							{
								printf("Memória cheia - Impossível criar processo.\n");
								writeInFile(nfp, ordem, tempo);
								ciclo();
							}
							else 
							{ //Caso tenha memória disponível:

								procId++;
								struct pcb *x = newPcb(procId);

								//alterar valores.
								int inicio = pos - t + 1;
								x -> fim = inicio + nIns;
								x -> ini = inicio;
								x -> pc = inicio;
								x -> estado = 0;

								int w =0;
								for (int g = x -> ini; g <= x -> fim - 1 ; g++)
								{
									if (arrayI[w] != '0' )
									{
										arrayP[g] = arrayI[w];
										w++;	
									}
									else 
									{
										arrayP[g] = arrayI[w];
									}
								}//fim de encher arrayP.
								//inserir nas Qs.
								nProc++;
								insert(x, new);
								insert(x, ordem);
								writeInFile(nfp, ordem, tempo);
								fclose(fp);
							}
						}//fim da criação de novo processo.
					}
					else //número de processos igual ao máximo.
					{
						printf("Número máximo de ficheiros atingido.");
						writeInFile(nfp, ordem, tempo);
						ciclo();
					}
				}//fim de introduzir ficheiros.
				else //Até não haver ficheiros
				{
					while(nProc != -1) 
					{
						writeInFile(nfp, ordem, tempo);
						ciclo();
					}

					writeInFile(nfp, ordem, tempo);
					escreve = false;
					break;	
				}//acaba o pedido de ficheiros
			}//fim do pedido de ficheiros
		}//fim best fit
	}//fim escreve
	fclose(nfp);
	return 0;
}//fim main