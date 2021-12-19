#include<stdio.h>
int main(void)
{
    //variable declaration
    int i_SSP,j_SSP;

    //code
    printf("\n\n");
    i_SSP=1;
    while (i_SSP<=10)
    {
        /* code */
        printf("i = %d\n",i_SSP);
        printf("-------------------\n\n");

        j_SSP=1;
        while (j_SSP<=5)
        {
            /* code */
            printf("\tj_SSP=%d\n",j_SSP);
            j_SSP++;
        }
        i_SSP++;
        printf("\n\n");
    }
    return 0;
}
    