#include <stdio.h>
int main(void)
{
    //variable declaraions
    int iArray_SSP[5][3] = { {1, 2, 3}, {2, 4, 6}, {3, 6, 9}, {4, 8, 12}, {5, 10, 15} }; //IN-LINE INITIALIZATION
    int int_size_SSP;
    int iArray_size_SSP;
    int iArray_num_elements_SSP, iArray_num_rows_SSP, iArray_num_columns_SSP;
    int i_SSP, j_SSP;
    //code
    printf("\n\n");
    int_size_SSP = sizeof(int);
    iArray_size_SSP = sizeof(iArray_SSP);
    printf("Size Of Two Dimensional ( 2D ) Integer Array Is = %d\n\n", iArray_size_SSP);
    iArray_num_rows_SSP = iArray_size_SSP / sizeof(iArray_SSP[0]);
    printf("Number of Rows In Two Dimensional ( 2D ) Integer Array Is = %d\n\n", iArray_num_rows_SSP);
    iArray_num_columns_SSP = sizeof(iArray_SSP[0]) / int_size_SSP;
    printf("Number of Columns In Two Dimensional ( 2D ) Integer Array Is = %d\n\n", iArray_num_columns_SSP);
    iArray_num_elements_SSP = iArray_num_rows_SSP * iArray_num_columns_SSP;
    printf("Number of Elements In Two Dimensional ( 2D ) Integer Array Is = %d\n\n", iArray_num_elements_SSP);
    printf("\n\n");
    printf("Elements In The 2D Array : \n\n");
    // *** ARRAY INDICES BEGIN FROM 0, HENCE, 1ST ROW IS ACTUALLY 0TH ROW AND 1ST COLUMN IS ACTUALLY 0TH COLUMN ***
    for (i_SSP = 0; i_SSP < iArray_num_rows_SSP; i_SSP++)
    {   
        printf("****** ROW %d ******\n", (i_SSP + 1));
            for (j_SSP = 0; j_SSP < iArray_num_columns_SSP; j_SSP++)
            {
                printf("iArray[%d][%d] = %d\n", i_SSP, j_SSP, iArray_SSP[i_SSP][j_SSP]);
            }
        printf("\n\n");
    }
    return(0);
}