#ifndef H_RIGHTTRIANGLE
#define H_RIGHTTRIANGLE

struct rightTriangle
{
    float x1;
    float x2;
    float x3;
    float y1;
    float y2;
    float y3;
};

float rightTrianglePerimeter(struct rightTriangle triangle);

float rightTriangleArea(struct rightTriangle triangle);

#endif 