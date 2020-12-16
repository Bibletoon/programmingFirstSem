#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

enum commandFlag
{
    NONE,
    SHOW,
    GET,
    SET
};

typedef union id3v2Header
{
    char buffer[10];
    struct
    {
        unsigned char marker[3];
        unsigned char version;
        unsigned char subversion;
        unsigned char flags;
        unsigned int size;
    } data;
} tagHeader;

typedef union id3v2Frame
{
    char buffer[10];
    struct
    {
        unsigned char name[4];
        unsigned int size;
        unsigned short flags;
    } data;
} tagFrame;

unsigned int reverseBytes(unsigned int num) { return (((num & 0xFF000000) >> 24) | ((num & 0x00FF0000) >> 8) | ((num & 0x0000FF00) << 8) | ((num & 0x000000FF) << 24)); };

unsigned int getSize(unsigned int n)
{
    return (n % 128) + (((n >> 8) % 128) << 7) + (((n >> 16) % 128) << 14) + (((n >> 24) % 128) << 14);
}

void show(char *filename)
{
    FILE *file;
    if ((file = fopen(filename, "rb")) == NULL)
    {
        printf("Failed to open file");
        exit(404);
    }
    tagHeader header;
    fread(header.buffer, (size_t)1, 10, file);
    if (header.buffer[0] == -1)
    {
        printf("No metainformation in file.");
        return;
    }
    printf("%c%c%cv2.%d.%d\n", header.data.marker[0], header.data.marker[1], header.data.marker[2], header.data.version, header.data.subversion);
    if (header.data.version == 2) {
        printf("Unsupported version of file.");
        return;
    }
    while (ftell(file) < getSize(header.data.size) + 10)
    {
        tagFrame frame;
        fread(frame.buffer, (size_t)1, 10, file);
        if (frame.data.name[0] == 0)
        {
            break;
        }
        printf("%s = ", frame.data.name);

        unsigned int valueSize = reverseBytes(frame.data.size);
        unsigned char *value = malloc((size_t)1 * valueSize);
        fread(value, (size_t)1, (size_t)1 * valueSize, file);
        if (frame.data.name[0] == 'A' && frame.data.name[1] == 'P' && frame.data.name[2] == 'I' && frame.data.name[3] == 'C')
        {
            printf("unsupported tag\n");
        }
        else if (frame.data.name[0] == 'C' && frame.data.name[1] == 'O' && frame.data.name[2] == 'M' && frame.data.name[3] == 'M')
        {
            printf("unsupported tag\n");
        }
        else if (frame.data.name[0] == 'T' && frame.data.name[1] == 'T')
        {
            printf("unsupported tag\n");
        }
        else
        {
            for (int i = 0; i < valueSize; i++)
            {
                printf("%c", value[i]);
            }
        }

        printf("\n");
        free(value);
    }
    fclose(file);
}

void get(char *filename, char *propname)
{
    FILE *file;
    if ((file = fopen(filename, "rb")) == NULL)
    {
        printf("Failed to open file");
        exit(404);
    }
    tagHeader header;
    fread(header.buffer, (size_t)1, 10, file);
    if (header.data.version == 2) {
        printf("Unsupported version of file.");
        return;
    }
    while (ftell(file) < getSize(header.data.size) + 10)
    {
        tagFrame frame;
        fread(frame.buffer, (size_t)1, 10, file);
        if (frame.data.name[0] == 0)
        {
            break;
        }
        unsigned int valueSize = reverseBytes(frame.data.size);
        if (strcmp(propname, frame.data.name) == 0)
        {

            unsigned char *value = malloc((size_t)1 * valueSize);
            fread(value, (size_t)1, valueSize, file);
            if (frame.data.name[0] == 'A' && frame.data.name[1] == 'P' && frame.data.name[2] == 'I' && frame.data.name[3] == 'C')
            {
                printf("unsupported tag");
            }
            else if (frame.data.name[0] == 'C' && frame.data.name[1] == 'O' && frame.data.name[2] == 'M' && frame.data.name[3] == 'M')
            {
                printf("unsupported tag");
            }
            else if (frame.data.name[0] == 'T' && frame.data.name[1] == 'T')
            {
                printf("unsupported tag\n");
            }
            else
            {
                for (int i = 0; i < valueSize; i++)
                {
                    printf("%c", value[i]);
                }
            }
            fclose(file);
            free(value);
            return;
        }
        fseek(file, valueSize, SEEK_CUR);
    }
    fclose(file);
    printf("Value not found for %s!", propname);
}

void copyFile(FILE *first, FILE *second)
{
    int c;
    while ((c = getc(first)) != EOF)
    {
        putc(c, second);
    }
}

void set(char *filename, char *propname, char *value)
{
    FILE *file;
    if ((file = fopen(filename, "rb")) == NULL)
    {
        printf("Failed to open file");
        exit(404);
    }
    FILE *copy;
    copy = fopen("tmp.mp3", "wb");
    copyFile(file, copy);
    fclose(copy);
    fseek(file, 0, SEEK_SET);
    int oldFramePosition = 0;
    unsigned int oldValueSize = 0;
    tagHeader header;
    fread(header.buffer, (size_t)1, 10, file);

    int wasframe = 0;
    while (ftell(file) < getSize(header.data.size) + 10)
    {
        tagFrame frame;
        fread(frame.buffer, (size_t)1, 10, file);
        if (frame.data.name[0] == 0)
        {
            break;
        }
        unsigned int valueSize = reverseBytes(frame.data.size);
        if (strcmp(propname, frame.data.name) == 0)
        {
            oldFramePosition = ftell(file) - 10;
            oldValueSize = valueSize;
            wasframe = 1;
            break;
        }
        fseek(file, valueSize, SEEK_CUR);
    }
    unsigned int newValueSize = strlen(value);

    unsigned int newInHeaderSize;
    if (header.buffer[0] == -1)
    {
        newInHeaderSize = 20 + newValueSize;
    }
    else if (wasframe == 0)
    {
        newInHeaderSize = getSize(header.data.size) + 10 + newValueSize;
    }
    else
    {
        newInHeaderSize = getSize(header.data.size) - oldValueSize + newValueSize;
    }

    if (oldFramePosition == 0)
    {
        oldFramePosition = ftell(file) - 10;
    }
    if (newValueSize == 0)
    {
        newInHeaderSize -= 10;
    }
    fclose(file);

    file = fopen(filename, "wb");
    copy = fopen("tmp.mp3", "rb");

    tagHeader newHeader;
    newHeader.data.marker[0] = 73;
    newHeader.data.marker[1] = 68;
    newHeader.data.marker[2] = 51;
    newHeader.data.version = 3;
    newHeader.data.subversion = 0;
    newHeader.data.flags = 0;
    newHeader.buffer[9] = newInHeaderSize % 128;
    newHeader.buffer[8] = (newInHeaderSize >> 7) % 128;
    newHeader.buffer[7] = (newInHeaderSize >> 14) % 128;
    newHeader.buffer[6] = (newInHeaderSize >> 21) % 128;
    fwrite(newHeader.buffer, (size_t)1, 10, file);
    fseek(copy, 10, SEEK_SET);

    int ind = 10;
    while (ftell(copy) < oldFramePosition)
    {
        int c;
        c = fgetc(copy);
        fputc(c, file);
    }

    if (newValueSize > 0)
    {
        tagFrame newFrame;
        newFrame.data.name[0] = propname[0];
        newFrame.data.name[1] = propname[1];
        newFrame.data.name[2] = propname[2];
        newFrame.data.name[3] = propname[3];
        newFrame.data.size = reverseBytes(newValueSize + 1);
        newFrame.data.flags = 0;

        fwrite(newFrame.buffer, (size_t)1, 10, file);
        putc(0, file);
        fwrite(value, (size_t)1, newValueSize, file);
    }

    if (wasframe != 0)
    {
        fseek(copy, oldFramePosition + 10 + oldValueSize, SEEK_SET);
    }

    int c;
    while ((c = getc(copy)) != EOF)
    {
        putc(c, file);
    }

    fclose(file);
    fclose(copy);
    remove("tmp.mp3");
    printf("New %s value is %s", propname, value);
}

void checkShow(char *filename)
{
    if (filename == NULL)
    {
        printf("Error. Filename required!");
        exit(400);
    }
    show(filename);
}

void checkGet(char *filename, char *propname)
{
    if (filename == NULL)
    {
        printf("Error. Filename required!");
        exit(400);
    }
    else if (propname == NULL)
    {
        printf("Error. Propname required!");
        exit(400);
    }
    get(filename, propname);
}

void checkSet(char *filename, char *propname, char *value)
{
    if (filename == NULL)
    {
        printf("Error. Filename required!");
        exit(400);
    }
    else if (propname == NULL)
    {
        printf("Error. Propname required!");
        exit(400);
    }
    else if (value == NULL)
    {
        printf("Error. Value required!");
        exit(400);
    }
    set(filename, propname, value);
}

int main(int argc, char *argv[])
{
    char *filename = NULL;
    char *propname = NULL;
    char *value = NULL;
    enum commandFlag flag;
    for (int i = 1; i < argc; i++)
    {
        switch (argv[i][2])
        {
        case 'f':
            filename = strpbrk(argv[i], "=") + 1;
            break;
        case 's':
            if (argv[i][3] == 'h')
            {
                flag = SHOW;
            }
            else
            {
                flag = SET;
                propname = strpbrk(argv[i], "=") + 1;
            }
            break;
        case 'g':
            flag = GET;
            propname = strpbrk(argv[i], "=") + 1;
            break;
        case 'v':
            value = strpbrk(argv[i], "=") + 1;
            break;
        }
    }

    switch (flag)
    {
    case SHOW:
        checkShow(filename);
        break;
    case GET:
        checkGet(filename, propname);
        break;
    case SET:
        checkSet(filename, propname, value);
        break;
    case NONE:
        printf("No command were given.");
        break;
    }
}