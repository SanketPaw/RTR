#include<stdio.h>
int main(int argc,char*argv[],char*envp[])
{
    //variable Declarations
    unsigned int num_SSP;

    //functionn prototype
    void recursive(unsigned int);

    //code
    printf("\n\n");
    printf("Enter Any Number : \n\n");
    scanf("%u",&num_SSP);

    printf("\n\n");
    printf("Output Of Recursive Function : \n\n");

    recursive(num_SSP);
    return 0;
}

void recursive(unsigned int n_SSP)
{
    //code
    printf("n = %d",n_SSP);

    if(n_SSP>0)
    {
        recursive(n_SSP-1);
    }
}