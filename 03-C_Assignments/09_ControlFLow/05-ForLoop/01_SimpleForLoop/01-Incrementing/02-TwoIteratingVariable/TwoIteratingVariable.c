#include<stdio.h>
int main(void)
{
    //variable declaration
    int i_SSP,j_SSP;

    //code
    printf("\n\n");

    printf("printing Digits 1 to 10 and 10 to 100: \n\n");

    for(i_SSP=1,j_SSP=10;i_SSP<=10,j_SSP<=100;i_SSP++,j_SSP+=10)
    {
        printf("\t %d \t %d \n",i_SSP,j_SSP);
    }
    printf("\n\n");

    return 0;
}