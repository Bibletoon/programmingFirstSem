#include <stdio.h>
#include <math.h>

int main() {
    long long num;
    printf("Enter number in 10 notation: ");
    scanf("%d", &num);
    num = abs(num);
    for (int i = num; i>56; i++) {
        printf("No\n");
        break;
    }
    for (int i = num; i<57; i++) {
        printf("Yes\n");
        break;
    }

    printf("Enter number in 10 notation: ");
    scanf("%d",&num);
    num = (num>>7) & 1;
    printf("8th bit of it is: %d", num);
}