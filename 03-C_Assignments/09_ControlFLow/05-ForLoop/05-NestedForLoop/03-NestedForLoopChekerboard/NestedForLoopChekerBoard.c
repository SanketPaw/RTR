#include<stdio.h>
int main(void)
{
    int i_SSP,j_SSP,c_SSP;
    printf("\n\n");
    for(i_SSP=0;i_SSP<64;i_SSP++)
    {
        for (j_SSP = 0; j_SSP < 64; j_SSP++)
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
        }
        printf("\n\n");
    }
    return 0;
}