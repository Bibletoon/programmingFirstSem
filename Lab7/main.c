#include <stdio.h>
#include <math.h>

enum Lamp {
    FILAMENT,
    HALOGEN,
    FLURESCENT,
    LED,
};

struct triangle
{
    int x1;
    int y1;
    int x2;
    int y2;
    int x3;
    int y3;
    float P;
};

float CalculateTrianglePerimeter(struct triangle Figure) {
    float len = sqrt(pow((Figure.x2-Figure.x1),2)+pow((Figure.y2-Figure.y1),2));
    len += sqrt(pow((Figure.x3-Figure.x2),2)+pow((Figure.y3-Figure.y2),2));
    len += sqrt(pow((Figure.x1-Figure.x3),2)+pow((Figure.y1-Figure.y3),2));
    return len;
}

union cardReader
{
    char buffer[2];
    struct
    {
    unsigned isOn: 1;
    unsigned activeSD: 1;
    unsigned activeCF: 1;
    unsigned activeMS: 1;
    } data;
};

int main() {
    printf("Number of halogen lamp: %d\n", HALOGEN);

    struct triangle myTriangle;
    printf("Your figure is triangle\n");
    printf("Enter x1: ");
    scanf("%d",&myTriangle.x1);
    printf("Enter y1: ");
    scanf("%d",&myTriangle.y1);

    printf("Enter x2: ");
    scanf("%d",&myTriangle.x2);
    printf("Enter y2: ");
    scanf("%d",&myTriangle.y2);

    printf("Enter x3: ");
    scanf("%d",&myTriangle.x3);
    printf("Enter y3: ");
    scanf("%d",&myTriangle.y3);

    myTriangle.P = CalculateTrianglePerimeter(myTriangle);

    printf("Your triangle perimeter is %f\n",myTriangle.P);

    union cardReader myCardReader;

    printf("Enter digit in 16 notation: ");
    scanf("%x", &myCardReader);
    printf("Card reader status:\n");
    printf("Card reader: %s\n", (myCardReader.data.isOn) ? "ON" : "OFF");
    printf("SD card: %s\n", (myCardReader.data.activeSD) ? "ON" : "OFF");
    printf("CF card: %s\n", (myCardReader.data.activeCF) ? "ON" : "OFF");
    printf("MS card: %s\n", (myCardReader.data.activeMS) ? "ON" : "OFF");

}