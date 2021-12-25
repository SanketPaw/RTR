#include <stdio.h>
#define NUM_ROWS 5
#define NUM_COLUMNS 3
#define DEPTH 2
int main(void)
{
    //variable declaraions
    //IN-LINE INITIALIZATION
    int iArray_SSP[NUM_ROWS][NUM_COLUMNS][DEPTH] = { { { 9, 18 }, { 27, 36 }, { 45, 54 } },
                                                     { { 8, 16 }, { 24, 32 }, { 40, 48 } },
                                                     { { 7, 14 }, { 21, 28 }, { 35, 42 } }, 
                                                     { { 6, 12 }, { 18, 24 }, { 30, 36 } },
                                                     { { 5, 10 }, { 15, 20 }, { 25, 30 } } };
    int i_SSP, j_SSP, k_SSP;
    int iArray_1D_SSP[NUM_ROWS * NUM_COLUMNS * DEPTH]; // 5 * 3 * 2 ELEMENTS => 30 ELEMENTS IN 1D ARRAY

    //code
    // ****** DISPLAY 3D ARRAY ******
    printf("\n\n");
    printf("Elements In The 3D Array : \n\n");
    for (i_SSP = 0; i_SSP < NUM_ROWS; i_SSP++)
    {   
        printf("****** ROW %d ******\n", (i_SSP + 1));
        for (j_SSP = 0; j_SSP < NUM_COLUMNS; j_SSP++)
        {
            printf("****** COLUMN %d ******\n", (j_SSP + 1));
            for (k_SSP = 0; k_SSP < DEPTH; k_SSP++)
            {
                printf("iArray[%d][%d][%d] = %d\n", i_SSP, j_SSP, k_SSP, iArray_SSP[i_SSP][j_SSP][k_SSP]);
            }
            printf("\n");
        }
        printf("\n");
    }
    // ****** CONVERTING 3D TO 1D ******
    for (i_SSP = 0; i_SSP < NUM_ROWS; i_SSP++)
    {
        for (j_SSP = 0; j_SSP < NUM_COLUMNS; j_SSP++)   
        {
            for (k_SSP = 0; k_SSP < DEPTH; k_SSP++)
            {
                iArray_1D_SSP[(i_SSP * NUM_COLUMNS * DEPTH) + (j_SSP * DEPTH) + k_SSP] = iArray_SSP[i_SSP][j_SSP][k_SSP];
            }
        }
    }
    // ****** DISPLAY 1D ARRAY ******
    printf("\n\n\n\n");
    printf("Elements In The 1D Array : \n\n");
    for (i_SSP = 0; i_SSP < (NUM_ROWS * NUM_COLUMNS * DEPTH); i_SSP++)
    {
        printf("iArray_1D[%d] = %d\n", i_SSP, iArray_1D_SSP[i_SSP]);
    }
    return(0);
}
