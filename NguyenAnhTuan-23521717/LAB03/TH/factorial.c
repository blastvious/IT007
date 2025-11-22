/*######################################
# University of Information Technology #
# IT007 Operating System
#
# <NGUYEN ANH TUAN>, <23521717> #
# File: factorial.c
#
######################################*/
#include <stdio.h>
int main()
{
int i, num, j;
printf("Enter the number: ");
scanf ("%d", &num );
for (i=1; i<num; i++)
j=j*i;
printf("The factorial of %d is %d\n", num, j);
return 0;
}