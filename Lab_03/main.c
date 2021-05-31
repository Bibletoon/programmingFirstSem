#include <stdio.h>

int main() {
    int num;
    printf("Enter number in 10 notation: ");
    scanf("%d", &num);
    printf("This num in 8 notation: %o \n", num);
    printf("This num in 16 notation %x \n", num);
    int numSd = num >> 4;
    printf("This num in 16 notation, with 4 bit right shift: %x \n", numSd);
    int numOt = ~num;
    printf("This num with bitwise NOT: %x\n",numOt);
    int numTwo;
    printf("Enter number in 16 notation: ");
    scanf("%x",&numTwo);
    
    int res = num ^ numTwo;
    printf("Resust of XOR of first and second number: %x", res);
}