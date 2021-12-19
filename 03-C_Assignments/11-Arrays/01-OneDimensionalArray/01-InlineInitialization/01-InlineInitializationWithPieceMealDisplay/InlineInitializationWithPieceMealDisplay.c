#include <stdio.h>
int main(void)
{
    //variable declaraions
    int iArray_SSP[] = { 9, 30, 6, 12, 98, 95, 20, 23, 2, 45 };
    int int_size_SSP;
    int iArray_size_SSP;
    int iArray_num_elements_SSP;
    float fArray_SSP[] = { 1.2f, 2.3f, 3.4f, 4.5f, 5.6f, 6.7f, 7.8f, 8.9f };
    int float_size_SSP;
    int fArray_size_SSP;
    int fArray_num_elements_SSP;
    char cArray_SSP[] = { 'A', 'S', 'T', 'R', 'O', 'M', 'E', 'D', 'I', 'C', 'O', 'M', 'P' };
    int char_size_SSP;
    int cArray_size_SSP;
    int cArray_num_elements_SSP;
    //code
    // ****** iArray[] ******
    printf("\n\n");
    printf("In-line Initialization And Piece-meal Display Of Elements of Array 'iArray[]': \n\n");
    printf("iArray[0] (1st Element) = %d\n", iArray_SSP[0]);
    printf("iArray[1] (2nd Element) = %d\n", iArray_SSP[1]);    
    printf("iArray[2] (3rd Element) = %d\n", iArray_SSP[2]);
    printf("iArray[3] (4th Element) = %d\n", iArray_SSP[3]);
    printf("iArray[4] (5th Element) = %d\n", iArray_SSP[4]);
    printf("iArray[5] (6th Element) = %d\n", iArray_SSP[5]);
    printf("iArray[6] (7th Element) = %d\n", iArray_SSP[6]);
    printf("iArray[7] (8th Element) = %d\n", iArray_SSP[7]);
    printf("iArray[8] (9th Element) = %d\n", iArray_SSP[8]);
    printf("iArray[9] (10th Element) = %d\n\n", iArray_SSP[9]);
    int_size_SSP = sizeof(int);
    iArray_size_SSP = sizeof(iArray_SSP);
    iArray_num_elements_SSP = iArray_size_SSP / int_size_SSP;
    printf("Size Of Data type 'int' = %d bytes\n", int_size_SSP);
    printf("Number Of Elements In 'int' Array 'iArray[]' = %d Elements\n", iArray_num_elements_SSP);
    printf("Size Of Array 'iArray[]' (%d Elements * %d Bytes) = %d Bytes\n\n", iArray_num_elements_SSP, int_size_SSP, iArray_size_SSP);
    // ****** fArray[] ******
    printf("\n\n");
    printf("In-line Initialization And Piece-meal Display Of Elements of Array 'fArray[]': \n\n");
    printf("fArray[0] (1st Element) = %f\n", fArray_SSP[0]);
    printf("fArray[1] (2nd Element) = %f\n", fArray_SSP[1]);
    printf("fArray[2] (3rd Element) = %f\n", fArray_SSP[2]);
    printf("fArray[3] (4th Element) = %f\n", fArray_SSP[3]);
    printf("fArray[4] (5th Element) = %f\n", fArray_SSP[4]);
    printf("fArray[5] (6th Element) = %f\n", fArray_SSP[5]);
    printf("fArray[6] (7th Element) = %f\n", fArray_SSP[6]);
    printf("fArray[7] (8th Element) = %f\n", fArray_SSP[7]);
    printf("fArray[8] (9th Element) = %f\n", fArray_SSP[8]);
    printf("fArray[9] (10th Element) = %f\n\n", fArray_SSP[9]);
    float_size_SSP = sizeof(float);
    fArray_size_SSP = sizeof(fArray_SSP);
    fArray_num_elements_SSP = fArray_size_SSP / float_size_SSP;
    printf("Size Of Data type 'float' = %d bytes\n", float_size_SSP);
    printf("Number Of Elements In 'float' Array 'fArray[]' = %d Elements\n", fArray_num_elements_SSP);
    printf("Size Of Array 'fArray[]' (%d Elements * %d Bytes) = %d Bytes\n\n",fArray_num_elements_SSP, float_size_SSP, fArray_size_SSP);
    // ****** cArray[] ******
    printf("\n\n");
    printf("In-line Initialization And Piece-meal Display Of Elements of Array 'cArray[]': \n\n");
    printf("cArray[0] (1st Element) = %c\n", cArray_SSP[0]);
    printf("cArray[1] (2nd Element) = %c\n", cArray_SSP[1]);
    printf("cArray[2] (3rd Element) = %c\n", cArray_SSP[2]);
    printf("cArray[3] (4th Element) = %c\n", cArray_SSP[3]);    
    printf("cArray[4] (5th Element) = %c\n", cArray_SSP[4]);
    printf("cArray[5] (6th Element) = %c\n", cArray_SSP[5]);
    printf("cArray[6] (7th Element) = %c\n", cArray_SSP[6]);
    printf("cArray[7] (8th Element) = %c\n", cArray_SSP[7]);
    printf("cArray[8] (9th Element) = %c\n", cArray_SSP[8]);
    printf("cArray[9] (10th Element) = %c\n", cArray_SSP[9]);
    printf("cArray[10] (11th Element) = %c\n", cArray_SSP[10]);
    printf("cArray[11] (12th Element) = %c\n", cArray_SSP[11]);
    printf("cArray[12] (13th Element) = %c\n\n", cArray_SSP[12]);
    char_size_SSP = sizeof(char);
    cArray_size_SSP = sizeof(cArray_SSP);
    cArray_num_elements_SSP = cArray_size_SSP / char_size_SSP;
    printf("Size Of Data type 'char' = %d bytes\n", char_size_SSP);
    printf("Number Of Elements In 'char' Array 'cArray[]' = %d Elements\n", cArray_num_elements_SSP);
    printf("Size Of Array 'cArray[]' (%d Elements * %d Bytes) = %d Bytes\n\n", cArray_num_elements_SSP, char_size_SSP, cArray_size_SSP);
    return(0);
}