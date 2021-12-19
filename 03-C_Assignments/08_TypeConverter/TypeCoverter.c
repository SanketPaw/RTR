#include <stdio.h>
int main(void)
{
    //variable declarations
    int i_SSP, j_SSP;
    char ch_01_SSP, ch_02_SSP;
    int a_SSP, result_int_SSP;
    float f_SSP, result_float_SSP;
    int i_explicit_SSP;
    float f_explicit_SSP;
    //code
    printf("\n\n");
    //INTERCONVERSION AND IMPLICIT TYPE-CASTING BETWEEN 'char' AND 'int' TYPES....
    i_SSP = 70;
    ch_01_SSP = i_SSP;
    printf("I_SSP = %d\n", i_SSP);
    printf("Charater 1 (after ch_01_SSP = i_SSP) = %c\n\n", ch_01_SSP);
    ch_02_SSP = 'Q_SSP';
    j_SSP = ch_02_SSP;
    printf("Charater 2 = %c\n", ch_02_SSP);
    printf("J_SSP (after j = ch_02_SSP) = %d\n\n", j_SSP);
    //IMPLICIT CONVERSION OF 'int' TO 'float'...
    a_SSP = 5;
    f_SSP = 7.8f;
    result_float_SSP = a_SSP + f_SSP;
    printf("Integer a_SSP = %d And Floating-Point Number %f Added Gives Floating-Point Sum = %f\n\n", a_SSP, f_SSP, result_float_SSP);
    result_int_SSP = a_SSP + f_SSP;
    printf("Integer a_SSP = %d And Floating-Point Number %f Added Gives Integer Sum_SSP = %d\n\n", a_SSP, f_SSP, result_int_SSP);
    //EXPLICIT TYPE-CASTING USING CAST OPERATOR...
    f_explicit_SSP = 30.121995f;
    i_explicit_SSP = (int)f_explicit_SSP;
    printf("Floating Point Number Which Will Be Type Casted Explicitly = %f\n",f_explicit_SSP);
    printf("Resultant Integer After Explicit Type Casting Of %f = %d\n\n", f_explicit_SSP, i_explicit_SSP);

    return(0);
}