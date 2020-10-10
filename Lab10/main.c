#include <stdio.h>
#include <math.h>
#include <string.h>

void moveString(char string[], int index) {
    int i = index;
    while (string[i])
    {
        if (i==99) {
            string[i] = 0;
            break;
        }
        string[i] = string[i+1];
        i++;
    }
    
}

void removeExtraSpaces(char string[],int index, int wasSpace) {
    int sw = (int)string[index];
    switch (sw)
    {
    case 32:
        if (wasSpace == 1) {
            moveString(string,index);
        } else {
            index++;
            wasSpace = 1;
        }
        break;
    case 0:
        return;
        break;
    default:
        wasSpace = 0;
        index++;
        break;
    }
    removeExtraSpaces(string,index,wasSpace);
}

int main()
{
    int n;
    printf("Enter natural number: ");
    scanf("%d", &n);
    int len = ceil(log10(n + 0.5));
    int digits[len];
    for (int i = len - 1; i >= 0; i--)
    {
        digits[i] = n % 10;
        n = n / 10;
    }
    printf("digits = [");
    for (int i = 0; i < len; i++)
    {
        printf(" %d,", digits[i]);
    }
    printf("]\n");
    char trash; //This variable uses to catch enter after input of number. If you remove it, fgets wil scan only enter symbol.
    scanf("%c",&trash);
    char string[100];
    printf("Enter string: ");
    fgets(string,256,stdin);
    int i=0;
    removeExtraSpaces(string,0,0);
    printf("New string:");
    while (string[i])
    {
        printf("%c",string[i]);
        i++;
    }
    return 0;
}