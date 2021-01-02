#include <stdio.h>
#include <malloc.h>

int main() {
    int array[4];
    int *pointer = array;

    array[0] = 1000;
    array[1] = 1001;
    array[2] = 1002;
    array[3] = 1003;

    for (int i = 0;i<4;i++) {
        printf("%d ", *(pointer+i));
    }
    printf("\n");

    int *dynamicArray = (int*)malloc(4 * sizeof(int));
    dynamicArray[0] = 1000;
    dynamicArray[1] = 1001;
    dynamicArray[2] = 1002;
    dynamicArray[3] = 1003;

    for (int i = 0;i<4;i++) {
        printf("%d ", dynamicArray[i]);
    }
    free(dynamicArray);
    printf("\n");

}