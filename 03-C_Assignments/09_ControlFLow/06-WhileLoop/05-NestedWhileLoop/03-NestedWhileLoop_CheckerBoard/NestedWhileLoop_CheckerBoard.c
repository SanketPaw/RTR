#include<stdio.h>
int main(void)
{
    int i_SSP,j_SSP,c_SSP;
    printf("\n\n");

    i_SSP=0;
    while(i_SSP<64)
    {
        j_SSP=0;
        while(j_SSP < 64)
        {
            c_SSP=((i_SSP & 0x8)==0) ^ ((j_SSP & 0x8)==0);
            if (c_SSP==0)
            {
                printf("  ");
            }
            if (c_SSP==1)
            {
                printf("* ");
            }
            j_SSP++;
        }
        printf("\n\n");
        i_SSP++;
    }
    return 0;
}