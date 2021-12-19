#include <stdio.h>
#define MAX_STRING_LENGTH 512
int main(void)
{
    //function prototype
    int MyStrlen(char[]);
    //variable declaraions
    // *** A 'STRING' IS AN ARRAY OF CHARACTERS ... so char[] IS A char ARRAY AND HENCE, char[] IS A 'STRING' ***
    // *** AN ARRAY OF char ARRAYS IS AN ARRAY OF STRINGS !!! ***
    // *** HENCE, char[] IS ONE char ARRAY AND HENCE, IS ONE STRING ***
    // *** HENCE, char[][] IS AN ARRAY OF char ARRAYS AND HENCE, IS AN ARRAY OF STRINGS ***
    //Here, the string array can allow a maximum number of 10 strings (10 rows) and each of these 10 strings can have only upto 15 characters maximum (15 columns)
    char strArray_SSP[10][15] = { "Hello!", "Welcome", "To", "Real", "Time", "Rendering", "Batch", "(2020-21)", "Of", "ASTROMEDICOMP." }; //IN-LINE INITIALIZATION
    int iStrLengths_SSP[10]; //1D Integer Array - Stores lengths of those strings at corressponding indices in 'strArray[]' e.g: iStrLengths[0] will be the length of string at strArray[0], iStrLengths[1] will be the length of string at strArray[1]...10 strings, 10 lengths...
    int strArray_size_SSP;
    int strArray_num_rows_SSP;
    int i_SSP, j_SSP;
    //code
    strArray_size_SSP = sizeof(strArray_SSP);
    strArray_num_rows_SSP = strArray_size_SSP / sizeof(strArray_SSP[0]);
    //Storing in lengths of all the strings...
    for (i_SSP = 0; i_SSP < strArray_num_rows_SSP; i_SSP++)
        iStrLengths_SSP[i_SSP] = MyStrlen(strArray_SSP[i_SSP]);
        printf("\n\n");
        printf("The Entire String Array : \n\n");

    for (i_SSP = 0; i_SSP < strArray_num_rows_SSP; i_SSP++) 
    printf("%s ", strArray_SSP[i_SSP]);
    printf("\n\n");
    printf("Strings In The 2D Array : \n\n");
    //Since, char[][] is an array of strings, referencing only by the row number (first []) will give the row or the string
    //The Column Number (second []) is the particular character in that string / row
    for (i_SSP = 0; i_SSP < strArray_num_rows_SSP; i_SSP++)
    {
        printf("String Number %d => %s\n\n", (i_SSP + 1), strArray_SSP[i_SSP]);
        for (j_SSP = 0; j_SSP < iStrLengths_SSP[i_SSP]; j_SSP++)
        {
            printf("Character %d = %c\n", (j_SSP + 1), strArray_SSP[i_SSP][j_SSP]);
        }
        printf("\n\n");
    }
    return(0);
}
int MyStrlen(char str[])
{
    //variable declarations
    int j_SSP;
    int string_length_SSP = 0;
    //code
    // *** DETERMINING EXACT LENGTH OF THE STRING, BY DETECTING THE FIRST OCCURENCE OF NULL-TERMINATING CHARACTER ( \0 ) ***
    for (j_SSP = 0; j_SSP < MAX_STRING_LENGTH; j_SSP++)
    {
        if (str[j_SSP] == '\0')
        break;
        else
        string_length_SSP++;
    }
    return(string_length_SSP);
}
