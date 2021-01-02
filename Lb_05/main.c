#include <stdio.h>

int main() {
    int arr[8] = {77, 12, 74, 34, 56, 78, 234, 678};
    printf("array = [");
    for (int i=0; i<7; i++) {
        if (i!=6) {
            printf(" %d,", arr[i]);
        } else {
            printf("%d]\n", arr[i]);
        }
    }

    int matrixA[2][2] = {{2,1},{1,3}};
    int matrixB[2][2] = {{1,2},{3,1}};
    int res[2][2] = {{0,0},{0,0}};
    for (int i=0;i<sizeof(matrixA)/sizeof(*matrixA);i++) {
        for (int j=0;j<sizeof(matrixB[0])/sizeof(*matrixB[0]);j++) {
            int s = 0;
            for (int k=0;k<sizeof(matrixB)/sizeof(*matrixB);k++) {
                s+=matrixA[i][k]*matrixB[k][j];
            }
            res[i][j] = s;
        }
    }
    for (int i=0; i<sizeof(res)/sizeof(*res); i++) {
        for (int j=0;j<sizeof(res[0])/sizeof(*res[0]);j++) {
            printf("%d ",res[i][j]);
        }
    printf("\n");
    }
}

