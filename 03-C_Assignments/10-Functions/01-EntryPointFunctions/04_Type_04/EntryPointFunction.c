#include<stdio.h>   //'stdio.h' contains declaration of printf()

//Entry point function=> main()=>Valid Return Type(int) and 2 parameter (int argc,char*argv[])
int main(int argc,char*argv[])
{
    int i_SSP;

    //code
    printf("\n\n");
    printf("Hello World !!!\n");    //Library Function
    printf("Number of  command Line Argumrnts =%d\n\n",argc);

    printf("Command line Arguments Passed To this Program Are : \n\n");
    for ( i_SSP = 0; i_SSP < argc; i_SSP++)
    {
        /* code */
        printf("Command Line Argument Number %d = %s\n",(i_SSP+1),argv[i_SSP]);
    }
    printf("\n\n");
    return 0;
}
