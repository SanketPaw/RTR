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
    int i_SSP;
    //code
    // ****** iArray[] ******
    printf("\n\n");
    printf("In-line Initialization And Loop (for) Display Of Elements of Array 'iArray[]': \n\n");
    int_size_SSP = sizeof(int);
    iArray_size_SSP = sizeof(iArray_num_elements_SSP);
    iArray_num_elements_SSP = iArray_size_SSP / int_size_SSP;
    for (i_SSP = 0; i_SSP < iArray_num_elements_SSP; i_SSP++)
    {
        printf("iArray[%d] (Element %d) = %d\n", i_SSP, (i_SSP + 1), iArray_SSP[i_SSP]);
    }
    printf("\n\n");
    printf("Size Of Data type 'int' = %d bytes\n", int_size_SSP);
    printf("Number Of Elements In 'int' Array 'iArray[]' = %d Elements\n", iArray_num_elements_SSP);
    printf("Size Of Array 'iArray[]' (%d Elements * %d Bytes) = %d Bytes\n\n", iArray_num_elements_SSP, int_size_SSP, iArray_size_SSP);
    // ****** fArray[] ******
    printf("\n\n");
    printf("In-line Initialization And Loop (while) Display Of Elements of Array'fArray[]': \n\n");
    float_size_SSP = sizeof(float);
    fArray_size_SSP = sizeof(fArray_SSP);
    fArray_num_elements_SSP = fArray_size_SSP / float_size_SSP; 
    i_SSP = 0;
    while(i_SSP < fArray_num_elements_SSP)
    {
        printf("fArray[%d] (Element %d) = %f\n", i_SSP, (i_SSP + 1), fArray_SSP[i_SSP]);
        i_SSP++;
    }
    printf("\n\n");
    printf("Size Of Data type 'float' = %d bytes\n", float_size_SSP);
    printf("Number Of Elements In 'float' Array 'fArray[]' = %d Elements\n", fArray_num_elements_SSP);
    printf("Size Of Array 'fArray[]' (%d Elements * %d Bytes) = %d Bytes\n\n",fArray_num_elements_SSP, float_size_SSP, fArray_size_SSP);
    // ****** cArray[] ******
    printf("\n\n");
    printf("In-line Initialization And Loop (do while) Display Of Elements of Array 'cArray[]': \n\n");
    char_size_SSP = sizeof(char);
    cArray_size_SSP = sizeof(cArray_SSP);
    cArray_num_elements_SSP = cArray_size_SSP / char_size_SSP;
    i_SSP = 0;
    do  
    {
        printf("cArray[%d] (Element %d) = %c\n", i_SSP, (i_SSP + 1), cArray_SSP[i_SSP]);
        i_SSP++;
    }while (i_SSP < cArray_num_elements_SSP);
    printf("\n\n");
    printf("Size Of Data type 'char' = %d bytes\n", char_size_SSP);
    printf("Number Of Elements In 'char' Array 'cArray[]' = %d Elements\n", cArray_num_elements_SSP);
    printf("Size Of Array 'cArray[]' (%d Elements * %d Bytes) = %d Bytes\n\n", cArray_num_elements_SSP, char_size_SSP, cArray_size_SSP);
    return(0);
}