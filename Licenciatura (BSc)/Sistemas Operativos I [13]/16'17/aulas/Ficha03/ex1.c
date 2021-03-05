#include <math.h>
#include <stdio.h>




int main(){

	int n, i;
	double sum = 0;
    
    printf("Enter a num: ");
    scanf("%d",&n);
//    printf("%d",n);


    for(i=1; i <= n; ++i){
    	sum += (double)sqrt(i);
    }

    printf("O total da soma de todas as raizes ate %d é: %lf", n, sum);

	return 0;
}