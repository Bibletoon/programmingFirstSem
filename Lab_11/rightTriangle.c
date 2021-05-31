#include "rightTriangle.h"
#include <math.h>

float rightTriangleArea(struct rightTriangle triangle) {
    float a = sqrt(pow((triangle.x2-triangle.x1),2)+pow((triangle.y2-triangle.y1),2));
    float b = sqrt(pow((triangle.x3-triangle.x2),2)+pow((triangle.y3-triangle.y2),2));
    float c = sqrt(pow((triangle.x1-triangle.x3),2)+pow((triangle.y1-triangle.y3),2));
    float p = (a+b+c)/2;
    float s = sqrt(p*(p-a)*(p-b)*(p-c));
    return s;
}

float rightTrianglePerimeter(struct rightTriangle triangle) {
    float len = sqrt(pow((triangle.x2-triangle.x1),2)+pow((triangle.y2-triangle.y1),2));
    len += sqrt(pow((triangle.x3-triangle.x2),2)+pow((triangle.y3-triangle.y2),2));
    len += sqrt(pow((triangle.x1-triangle.x3),2)+pow((triangle.y1-triangle.y3),2));
    return len;
}