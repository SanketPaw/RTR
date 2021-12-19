#include<stdio.h>

#define MY_PI 3.1415926535897932

#define AMC_STRING "AstroMediComp RTR"

//If First Constant Is Not Assigned A Value, It Is Assumed To Be 0 i.e: 'SUNDAY' Will Be 0
//And The Rest Of The Constants Are Assigned Consecutive Integer Values From 0 onwards i.e: 'MONDAY' Will Be 1, 'TUESDAY' Will Be 2, and so on...

//UN-named enums
enum
{
    Sunday_SSP,
    Monday_SSP,
    Tuesday_SSP,
    Wednesday_SSP,
    Thursday_SSP,
    Friday_SSP,
    Saturday_SSP
};

enum
{
    January_SSP=1,
    February_SSP,
    March_SSP,
    April_SSP,
    May_SSP,
    June_SSP,
    July_SSP,
    August_SSP,
    September_SSP,
    October_SSP,
    November_SSP,
    December_SSP
};

//named enums
enum
{
    One_SSP,
    Two_SSP,
    Three_SSP,
    Four_SSP,
    Five_SSP,
    Six_SSP,
    Seven_SSP,
    Eight_SSP,
    Nine_SSP,
    Ten_SSP
};

enum boolean
{
    TRUE=1,
    FALSE=0
};

int main (void)
{
    //Local constants declaration
    const double epsilon_SSP=0.000001;

    //code 
    printf("\n\n");
    printf("Local constant epsilon_SSP=%f\n\n",epsilon_SSP);

    printf("Sunday_SSP is day number = %d\n",Sunday_SSP);
    printf("Monday_SSP is day number = %d\n",Monday_SSP);
    printf("Tuesday_SSP is day number = %d\n",Tuesday_SSP);
    printf("Wednesday_SSP is day number = %d\n",Wednesday_SSP);
    printf("Thursday_SSP is day number = %d\n",Thursday_SSP);
    printf("Friday_SSP is day number = %d\n",Friday_SSP);
    printf("Saturday_SSP is day number = %d\n",Saturday_SSP);

    printf("One_SSP enum Number = %d\n",One_SSP);
    printf("Two_SSP enum Number = %d\n",Two_SSP);
    printf("Three_SSP enum Number = %d\n",Three_SSP);
    printf("Four_SSP enum Number = %d\n",Four_SSP);
    printf("Five_SSP enum Number = %d\n",Five_SSP);
    printf("Six_SSP enum Number = %d\n",Six_SSP);
    printf("Seven_SSP enum Number = %d\n",Seven_SSP);
    printf("Eight_SSP enum Number = %d\n",Eight_SSP);
    printf("Nine_SSP enum Number = %d\n",Nine_SSP);
    printf("Ten_SSP enum Number = %d\n",Ten_SSP);

    printf("January_SSP is month number = %d\n",January_SSP);
    printf("February_SSP is month number = %d\n",February_SSP);
    printf("March_SSP is month number = %d\n",March_SSP);
    printf("April_SSP is month number = %d\n",April_SSP);
    printf("May_SSP is month number = %d\n",May_SSP);
    printf("June_SSP is month number = %d\n",June_SSP);
    printf("July_SSP is month number = %d\n",July_SSP);
    printf("August_SSP is month number = %d\n",August_SSP);
    printf("September_SSP is month number = %d\n",September_SSP);
    printf("Octobe_SSP is month number = %d\n",October_SSP);
    printf("November_SSP is month number = %d\n",November_SSP);
    printf("December_SSP is month number = %d\n",December_SSP);

    printf("value of TRUE = %d\n",TRUE);
    printf("value of FALSE = %d\n\n",FALSE);

    printf("MY_PI macro Value =%.10lf\n\n,MY_PI");
    printf("Area Of Circle Of Radius 2 units = %f\n\n", (MY_PI * 2.0f * 2.0f)); //pi * r * r = area of circle of radius 'r'

    printf("\n\n");
    
    printf(AMC_STRING);
    printf("\n\n");
    
    printf("AMC_STRING is : %s\n", AMC_STRING);
    printf("\n\n");

    return 0;
}
