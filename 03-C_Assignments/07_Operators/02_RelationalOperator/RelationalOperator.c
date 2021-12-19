#include <stdio.h>
int main(void)
{
    //variable declarations
    int a_SSP;
    int b_SSP;
    int result_SSP;
    //code
    printf("\n\n");
    printf("Enter One Integer : ");
    scanf("%d", &a_SSP);
    printf("\n\n");
    printf("Enter Another Integer : ");
    scanf("%d", &b_SSP);

    printf("\n\n");

    printf("If Answer = 0, It Is 'FALSE'.\n");
    printf("If Answer = 1, It Is 'TRUE'.\n\n");

    result_SSP=(a_SSP < b_SSP);
    printf("(a_SSP < b_SSP) A_SSP = %d Is Less Than B_SSP = %d \t Answer = %d\n", a_SSP, b_SSP, result_SSP);

    result_SSP=(a_SSP > b_SSP);
    printf("(a_SSP > b_SSP) A_SSP = %d Is Greater Than B_SSP = %d \t Answer = %d\n", a_SSP, b_SSP, result_SSP);

    result_SSP=(a_SSP <= b_SSP);
    printf("(a_SSP <= b_SSP) A_SSP = %d Is Less Than Or Equal To B_SSP = %d \t Answer = %d\n",a_SSP, b_SSP, result_SSP);

    result_SSP=(a_SSP >= b_SSP);
    printf("(a_SSP >= b_SSP) A_SSP = %d Is Greater Than Or Equal To B_SSP = %d \t Answer = %d\n", a_SSP, b_SSP, result_SSP);

    result_SSP=(a_SSP == b_SSP);
    printf("(a_SSP == b_SSP) A_SSP = %d Is Equal To B_SSP = %d \t Answer = %d\n", a_SSP, b_SSP, result_SSP);

    result_SSP=(a_SSP != b_SSP);
    printf("(a_SSP != b_SSP) A_SSP = %d Is NOT Equal To B_SSP = %d \t Answer = %d\n", a_SSP, b_SSP, result_SSP);

    return(0);
}
