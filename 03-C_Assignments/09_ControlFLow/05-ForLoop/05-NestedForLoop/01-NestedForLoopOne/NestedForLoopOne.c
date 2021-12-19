#include<stdio.h>
int main(void)
{
    //variable declaration
    int i_SSP,j_SSP;

    //code
    printf("\n\n");
    for(i_SSP=1;i_SSP<=10;i_SSP++)
    {
        printf("i=%d\n",i_SSP);
        printf("-----------\n\n");
        for(j_SSP=1;j_SSP<=5;j_SSP++)
        {
            printf("\tj_SSP=%d\n",j_SSP);
        }
        printf("\n\n");
    }
    return 0;
}
