#include <stdio.h>

// Kevin Stephens Activity 1-2-2
int main() {
    printf("Enter an integer: ");
    int num;
    scanf("%d", &num);
    printf("You number in hexadecimal is: %x\n", num);
    printf("You number in octal is: %o\n", num);
    return 0;
}