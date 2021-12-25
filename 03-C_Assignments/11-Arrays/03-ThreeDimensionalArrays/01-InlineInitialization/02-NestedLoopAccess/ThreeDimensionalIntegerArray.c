#include <stdio.h>
int main(void)
{
    //variable declaraions
    //IN-LINE INITIALIZATION
    int iArray_SSP[5][3][2] = { { { 9, 18 }, { 27, 36 }, { 45, 54 } },
                            { { 8, 16 }, { 24, 32 }, { 40, 48 } },
                            { { 7, 14 }, { 21, 28 }, { 35, 42 } },
                            { { 6, 12 }, { 18, 24 }, { 30, 36 } },
                            { { 5, 10 }, { 15, 20 }, { 25, 30 } } };
    int int_size_SSP;
    int iArray_size_SSP;
    int iArray_num_elements_SSP, iArray_width_SSP, iArray_height_SSP, iArray_depth_SSP;
    int i_SSP, j_SSP, k_SSP;
    
    //code
    printf("\n\n");
    
    int_size_SSP = sizeof(int);
    
    iArray_size_SSP = sizeof(iArray_SSP);
    printf("Size Of Three Dimensional ( 3D ) Integer Array Is = %d\n\n", iArray_size_SSP);
    
    iArray_width_SSP = iArray_size_SSP / sizeof(iArray_SSP[0]);
    printf("Number of Rows (Width) In Three Dimensional ( 3D ) Integer Array Is = %d\n\n", iArray_width_SSP);
    
    iArray_height_SSP = sizeof(iArray_SSP[0]) / sizeof(iArray_SSP[0][0]);
    printf("Number of Columns (Height) In Three Dimensional ( 3D ) Integer Array Is = %d\n\n", iArray_height_SSP);
    
    iArray_depth_SSP = sizeof(iArray_SSP[0][0]) / int_size_SSP;
    printf("Depth In Three Dimensional ( 3D ) Integer Array Is = %d\n\n", iArray_depth_SSP);
    
    iArray_num_elements_SSP = iArray_width_SSP * iArray_height_SSP * iArray_depth_SSP;
    printf("Number of Elements In Three Dimensional ( 3D ) Integer Array Is = %d\n\n", iArray_num_elements_SSP);
    
    printf("\n\n");
    printf("Elements In Integer 3D Array : \n\n");
    for (i_SSP = 0; i_SSP < iArray_width_SSP; i_SSP++)
    {
        printf("****** ROW %d ******\n", (i_SSP + 1));
        for (j_SSP = 0; j_SSP < iArray_height_SSP; j_SSP++)
        {
            printf("****** COLUMN %d ******\n", (j_SSP + 1));
            for (k_SSP = 0; k_SSP < iArray_depth_SSP; k_SSP++)
            {
                printf("iArray[%d][%d][%d] = %d\n", i_SSP, j_SSP, k_SSP, iArray_SSP[i_SSP][j_SSP][k_SSP]);
            }
            printf("\n");
        }
        printf("\n\n");
    }
    return(0);
}
