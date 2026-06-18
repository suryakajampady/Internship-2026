#include<stdio.h>
// To find the sum of three numbers
//This comment is added to check the commit history
int main()
{
    int a, b, c, sum;
    printf("Enter three numbers: ");
    scanf("%d %d %d", &a, &b, &c);
    sum = a + b + c;
    printf("The sum of %d, %d and %d is: %d\n", a, b, c, sum);
    return 0;
}