#include <stdio.h>
#include <unistd.h>






    typedef struct pcb {
    int idProcessos;
    int tempoInicio
    int tempoCorrer
    int contador
};





//fork
/*

int main(int argc, char **argv)
{
    printf("--beginning of program\n");

    int counter = 0;
    pid_t pid = fork();

    if (pid == 0)
    {
        // child process
        int i = 0;
        for (; i < 5; ++i)
        {
            printf("child process: counter=%d\n", ++counter);
        }
    }
    else if (pid > 0)
    {
        // parent process
        int j = 0;
        for (; j < 5; ++j)
        {
            printf("parent process: counter=%d\n", ++counter);
        }
    }
    else
    {
        // fork failed
        printf("fork() failed!\n");
        return 1;
    }

    printf("--end of program--\n");

    return 0;
}

*/

int main()
	{
    		FILE *ptr_file;
    		char buf[1000]; //máximo chars por linha"

    		ptr_file =fopen("input.txt","r"); //nome do ficheiro a abrir
    		if (!ptr_file)
        		return 1;

    		while (fgets(buf,1000, ptr_file)!=NULL)
        		printf("%s",buf);

		fclose(ptr_file);
    		return 0;
	}


