#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define ARRAYMAXSIZE 100


int main() {

	char str[150];
	printf("Insira o nome do ficheiro: \n");
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

	//
	token = strtok(asdf, s);
	arrayQwerty[i] = atoi(token);

	//
	while( token != NULL ) {
	  printf( "%s\n", token );

	  token = strtok(NULL, s);
	  arrayQwerty[i] = atoi(token);
	  i++;
	}

	for (int i = 0; i < ARRAYMAXSIZE; i++)
		printf("%d\n", arrayQwerty[i]);

	fclose(fPointer);

	return(0);
}