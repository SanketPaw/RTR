#include<stdio.h>
int main(void)
{
    //variables declaration
    int i_SSP;

    printf("\n\n");

    printf("Printing Event Numbers From 0 to 100: \n\n");

    for(i_SSP=0;i_SSP<=100;i_SSP++)
    {
        /* code */
        //condition for number to be even => Division of a number by 2 leaves remainder 0
        //if remainder is not 0 it is a doo number...
        if (i_SSP%2 != 0)
        {
            /* code */
            continue;
        }
        else
        {
            printf("\t%d\n",i_SSP);
        }
    }
    printf("\n\n");

    return 0;
}