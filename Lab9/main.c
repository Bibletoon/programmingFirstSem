#include <stdio.h>

int main() {
    short digit;
    printf("Enter digit: ");
    scanf("%d",&digit);
    switch (digit)
    {
    case 0:
        printf("Zero");
        break;
    case 1:
        printf("One");
        break;
    case 2:
        printf("Two");
        break;
    case 3:
        printf("Three");
        break;
    case 4: 
        printf("Four");
        break;
    case 5:
        printf("Five");
        break;
    case 6:
        printf("Six");
        break;
    case 7:
        printf("Seven");
        break;
    case 8:
        printf("Eight");
        break;
    case 9:
        printf("Nine");
        break;
    default:
        printf("Wrong input");
        break;
    }
    printf("\n");
    float sum;
    float percent;
    int mounthCount;
    printf("Enter sum in bank: ");
    scanf("%f",&sum);
    printf("Enter percent (Example: 0.14): ");
    scanf("%g", &percent);
    printf("Enter count of mounthes: ");
    scanf("%d",&mounthCount);

    for (int i=0;i<mounthCount;i++) {
        sum+=sum*percent;
        printf("After %d mounth sum equals %f\n",i+1,sum);
    }
    return 0;
}