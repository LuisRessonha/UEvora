#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

/*

inicializar variáveis
temp=1
faz ciclo se for filho
temp=temp*n
n=n-1
se n=1 imprime "resultado é" temp, 
fork()

*/


#define fact 5

int main(void)
{
    int temp=1, i=1;
    pid_t pid;

    do
    {
        pid = fork();

        if (pid == 0)
        {
            temp *= i;
            ++i;
        }
    } while(i<=fact && pid == 0);

    if (pid == 0)
    {
        printf("O resultado é %d \n", temp);
    }
}