#include <stdio.h>

// Kevin Stephens Activity 1-2-3
int main() 
{
    int num;
    int bitPos;
    printf("Enter an integer: ");
    scanf("%d", &num);
    printf("Enter a bit position: ");
    scanf("%d", &bitPos);
    
    int mask = 1 << bitPos;
    num ^= mask;
    printf("Result: %d\n", num);

    return 0;
}