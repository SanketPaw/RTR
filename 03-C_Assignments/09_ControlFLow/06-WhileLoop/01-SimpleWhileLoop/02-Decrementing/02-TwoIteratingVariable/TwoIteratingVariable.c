#include<stdio.h>
int main(void)
{
    //variable declarationns
    int i_SSP,j_SSP;

    //code
    printf("\n\n");

    printf("Printing digits 10 to 1 and 100 to 10: \n\n");

    i_SSP=10;
    j_SSP=100;
    while(i_SSP>=1,j_SSP>=10)
    {
        printf("\t %d \t %d \n",i_SSP,j_SSP);
        i_SSP--;
        j_SSP=j_SSP-10;
    }

    printf("\n\n");

    return 0;
}