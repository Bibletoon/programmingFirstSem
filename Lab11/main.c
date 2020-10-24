#include <stdio.h>
#include "rightTriangle.c"

int main() {
    struct rightTriangle triangle;
    printf("Enter first point x: ");
    scanf("%f", &triangle.x1 );
    printf("Enter first point y: ");
    scanf("%f", &triangle.y1 );
    printf("Enter second point x: ");
    scanf("%f", &triangle.x2 );
    printf("Enter second point y: ");
    scanf("%f", &triangle.y2 );
    printf("Enter third point x: ");
    scanf("%f", &triangle.x3 );
    printf("Enter third point y: ");
    scanf("%f", &triangle.y3 );
    printf("%f %f %f\n", triangle.x1, triangle.x2, triangle.x3);
    printf("%f %f %f\n", triangle.y1, triangle.y2, triangle.y3);
    printf("Triangle area: %f\n", rightTriangleArea(triangle));
    printf("Triangle perimeter: %f\n", rightTrianglePerimeter(triangle));
}