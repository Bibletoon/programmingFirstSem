#include <stdio.h>
#include <math.h>

#define PI 3.14159265 

int main() {
    float x,y, z1, z2, z3;
    printf("Enter х: ");
    scanf("%f",&x);
    printf("Enter y: ");
    scanf("%f",&y);
    x = x*PI/180;
    y = y*PI/180;
    z1 = pow(sin(x),4)+pow(sin(y),2)+(pow(sin(2*x),2))/4-1;
    printf("First function value: %f",z1);
    z2 = sin(y+x)*sin(y-x);
    printf("\n");
    printf("Second function value: %f",z2);
    return 0;
}