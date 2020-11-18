#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

int strToNum(char *str)
{
    int num = 0;
    int i = 0;
    int isNegetive = 1;
    if (str[i] == '-')
    {
        isNegetive = -1;
        i++;
    }
    while (str[i] && (str[i] >= '0' && str[i] <= '9'))
    {
        num = num * 10 + (str[i] - '0');
        i++;
    }
    num = isNegetive * num;
    return num;
}

// .bmp файл
// Размер файла - Сдвиг 2 - размер 4
// Ширина картинки - Сдвиг 18 - размер 4
// Высота картинки - Сдвиг 22 - размер 4

int emulateGame(int **field, int height, int width)
{
    int temp[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp[i][j] = field[i][j];
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int numNeighbours = 0;
            int right = j == width - 1 ? 0 : j + 1;
            int left = j == 0 ? width - 1 : j - 1;
            int top = i == 0 ? height - 1 : i - 1;
            int bottom = i == height - 1 ? 0 : i + 1;

            numNeighbours += field[top][left] + field[top][j] + field[top][right] + field[i][left] + field[i][right] + field[bottom][left] + field[bottom][j] + field[bottom][right];
            if (field[i][j] == 0 && numNeighbours == 3)
            {
                temp[i][j] = 1;
            }
            else if (numNeighbours != 2 && numNeighbours != 3)
            {
                temp[i][j] = 0;
            }
        }
    }
    int numChanged = 0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (field[i][j] != temp[i][j])
            {
                numChanged += 1;
            }
            field[i][j] = temp[i][j];
        }
    }
    return numChanged == 0 ? 0 : 1;
}

void logField(int **field, int height, int width, int bitsPerPixel, int iteration, int imageStart, char *dirname, char *filename)
{
    FILE *image;
    image = fopen(filename, "rb");

    char newFilename[20];
    char newFilenameShort[500];
    strcpy(newFilename, dirname);
    strcat(newFilename, "\\");
    strcat(newFilename, itoa(iteration, newFilenameShort, 10));
    strcat(newFilename, ".bmp");

    FILE *newImage;
    newImage = fopen(newFilename, "wb");

    if (newImage == NULL)
    {
        printf("Can't create new file!");
        exit(-1);
    }

    int skipBytes = ((8 - width % 8) % 8) * bitsPerPixel / 8;
    int stringByteSize = width * bitsPerPixel / 8;
    unsigned char c;
    int k;
    for (int i = 0; i < imageStart; i++)
    {
        c = fgetc(image);
        fputc(c, newImage);
    }
    if (bitsPerPixel == 1)
    {
        for (int i=0;i<height;i++) {
            int j = 0;
            c=0;
            while (j<width) {
                c = c<<1;
                k = field[i][j];
                c+=field[i][j];
                j+=1;
                if (j!=0 && j%8==0) {
                    fputc(c,newImage);
                    c=0;
                } else if (j==width) {
                    c = c<<(8-(j%8));
                    j+=(8-j%8);
                    fputc(c,newImage);
                }
            }
            while (j%32!=0) {
                fputc(0,newImage);
                j+=8;
            }
        }
    }
    else
    {
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < stringByteSize; j++)
            {
                if (bitsPerPixel == 4)
                {
                    if (field[i][2 * j] == 1)
                    {
                        c = 15 << 4;
                    }
                    else
                    {
                        c = 0;
                    }
                    if (field[i][2 * j + 1] == 1)
                    {
                        c += 15;
                    }
                }
                else
                {
                    if (field[i][j] == 0)
                    {
                        c = 0;
                    }
                    else
                    {
                        c = 255;
                    }
                }
                fputc(c, newImage);
            }
            for (int j = 0; j < skipBytes; j++)
            {
                fputc(0, newImage);
            }
        }
    }
    fclose(image);
    fclose(newImage);
}

void loadFieldFEBpp(int **field, int height, int width, int bitsPerPixel, FILE *image)
{
    int skipBytes = ((8 - width % 8) % 8) * bitsPerPixel / 8;
    int stringByteSize = width * bitsPerPixel / 8;
    unsigned char c;
    printf("%d", skipBytes);
    for (int i = 0; i < height; i++)
    {
        int *fieldLine = (int *)malloc(width * sizeof(int));
        field[i] = fieldLine;
        for (int j = 0; j < stringByteSize; j++)
        {
            c = fgetc(image);
            if (bitsPerPixel == 4)
            {
                if (c >> 4 == 0)
                {
                    fieldLine[2 * j] = 0;
                }
                else
                {
                    fieldLine[2 * j] = 1;
                }
                if (c % 16 == 0)
                {
                    fieldLine[2 * j + 1] = 0;
                }
                else
                {
                    fieldLine[2 * j + 1] = 1;
                }
            }
            else if (bitsPerPixel == 8)
            {
                if (c == 0)
                {
                    fieldLine[j] = 0;
                }
                else
                {
                    fieldLine[j] = 1;
                }
            }
        }
        for (int j = 0; j < skipBytes; j++)
        {
            c = fgetc(image);
        }
    }
}

void loadFieldOneBpp(int **field, int height, int width, FILE *image)
{
    int bitsRead = 0;
    unsigned char c;
    for (int i = 0; i < height; i++)
    {
        int *fieldLine = (int *)malloc(width * sizeof(int));
        field[i] = fieldLine;
        bitsRead = 0;
        int nm;
        while (bitsRead < width)
        {
            c = fgetc(image);
            for (int j = 0; j < 8; j++)
            {
                nm = (c >> (7 - j)) % 2;
                if (nm==0) {fieldLine[bitsRead] = 0;} 
                else 
                {fieldLine[bitsRead]=1;}
                
                bitsRead += 1;
                if (bitsRead == width)
                {
                    bitsRead += 7 - j;
                    break;
                }
            }
        }
        while (bitsRead % 32 != 0)
        {
            c = fgetc(image);
            bitsRead += 8;
        }
    }
}

int main(int argc, char *argv[])
{

    char *filename = NULL;
    char *dirname = NULL;

    int dumpfreq = 1;
    int maxiter = 100;

    for (int i = 1; i < argc; i++)
    {
        switch (argv[i][2])
        {
        case 'i':
            filename = argv[i + 1];
            i++;
            break;
        case 'o':
            dirname = argv[i + 1];
            i++;
            break;
        case 'm':
            maxiter = strToNum(argv[i + 1]);
            i++;
            break;
        case 'd':
            dumpfreq = strToNum(argv[i + 1]);
            i++;
            break;
        }
    }
    if (filename == NULL || dirname == NULL)
    {
        printf("Input and output are required!");
        return -1;
    }

    FILE *image;

    image = fopen(filename, "rb");

    if (image == NULL)
    {
        printf("Failed to open file!");
        return -1;
    }

    char BITMAPFILEHEADER[14];
    char BITMAPINFO[40];
    unsigned char c;

    fread(BITMAPFILEHEADER, (size_t)1, 14, image);

    int imageStart = BITMAPFILEHEADER[10] + BITMAPFILEHEADER[11] * 256 + BITMAPFILEHEADER[12] * 256 * 256 + BITMAPFILEHEADER[13] * 256 * 256 * 256;

    fread(BITMAPINFO, (size_t)1, 40, image);

    int bitsPerPixel = BITMAPINFO[14] + BITMAPINFO[15] * 256;

    int width = BITMAPINFO[4] + BITMAPINFO[5] * 256 + BITMAPINFO[6] * 256 * 256 + BITMAPINFO[7] * 256 * 256 * 256;
    int height = BITMAPINFO[8] + BITMAPINFO[9] * 256 + BITMAPINFO[10] * 256 * 256 + BITMAPINFO[11] * 256 * 256 * 256;

    for (int i = 0; i < imageStart - 54; i++)
    {
        c = fgetc(image);
    }
    int **field = (int **)malloc(height * sizeof(int *));
    if (bitsPerPixel == 1)
    {
        loadFieldOneBpp(field, height, width, image);
    }
    else if (bitsPerPixel == 4 || bitsPerPixel == 8)
    {
        loadFieldFEBpp(field, height, width, bitsPerPixel, image);
    }
    else
    {
        printf("This image is not supported.");
        fclose(image);
        free(field);
        return -1;
    }
    fclose(image);
    mkdir(dirname);
    logField(field, height, width, bitsPerPixel, 0, imageStart, dirname, filename);
    for (int k = 1; k <= maxiter; k++)
    {
        printf("Starting iteration %d\n", k);
        int result = emulateGame(field, height, width);
        if (k % dumpfreq == 0)
        {
            logField(field, height, width, bitsPerPixel, k, imageStart, dirname, filename);
        }

        if (result == 0)
        {
            printf("Game over.");
            break;
        }
    }
    free(field);
    return 0;
}