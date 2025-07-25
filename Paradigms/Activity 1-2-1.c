#include <stdio.h>

// Kevin Stephens Activity 1-2-1
int main() {
    printf("Bits in a short: %d\n", sizeof(short) * 8);
    printf("Bits in an int: %d\n", sizeof(int) * 8);
    printf("Bits in a long: %d\n", sizeof(long) * 8);
    printf("Bits in a long long: %d\n", sizeof(long long) * 8);
    printf("Bits in a float: %d\n", sizeof(float) * 8);
    printf("Bits in a double: %d\n", sizeof(double) * 8);
    return 0;
}