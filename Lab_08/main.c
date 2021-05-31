#include <stdio.h>

int main() {
    char firstString[100];
    char secondString[100];
    printf("Enter first string: ");
    gets(firstString);
    printf("Enter second string: ");
    gets(secondString);
    int i=0;
    int j=0;
    char concatinatedString[200];
    while (firstString[i]) {
        concatinatedString[i] = firstString[i];
        i++;
    }
    while (secondString[j]) {
        concatinatedString[i]=secondString[j];
        i++;
        j++;
    } 
    printf("Concatinated string: %s\n", concatinatedString);

    i = 0;
    while (firstString[i]==secondString[i] && i<100 && firstString[i] && secondString[i]) {
        i++;
    }
    if (i!=100) {
        if (firstString[i]<secondString[i]) {
            printf("Strings are diferent in %d symbol, first string is lower than second.\n",i);
        } else if (firstString[i]>secondString[i]) {
            printf("Strings are diferent in %d symbol, first string is greater than second.\n",i);
        } else {
        printf("Strings are equal.\n");
        }
    } else {
        printf("Strings are equal.\n");
    }

    char symbol;
    printf("Enter symbol: "); 
    scanf("%c",&symbol);
    scanf("%c",&symbol);
    int ended = 0;
    i=0;
    while (firstString[i]) {
        if (firstString[i] == symbol) {
            ended = 1;
            break;
        }
        i++;
    }
    if (ended) {
        printf("Symbol was found in %d position\n",i);
    } else {
        printf("Symbol wasn't found.\n");
    }

    symbol;
    ended = 0;
    i=0;
    j=0;

    while (ended==0 && firstString[i]) {
        while (secondString[j]) {
            if (firstString[i] == secondString[j]) {
                ended = 1;
                break;
            }
            j++;
        }
        i++;
        j=0;
    }
    if (ended) {
        printf("Symbol %c from second string was found in first string in position %d\n",firstString[i-1],i-1);
    } else {
        printf("No one symbol from second string was found in first string.\n");
    }

    int len = 0;
    int maxLen = 0;
    i = 0;
    j=0;
    while (firstString[i]) {
        while (secondString[j])
        {
            if (firstString[i] == secondString[j]) {
                if (len>maxLen) {
                    maxLen = len;
                }
                len = 0;
                break;
            }
            j++;
        }
        len+=1;
        j=0;
        i++;
    }
    if (len>maxLen) {
        maxLen = len;
    }
    printf("Max substring without symbols of second stryng len is %d",maxLen);
    return 0;
}