#include <stdio.h>
#include <malloc.h>


enum tagFlag {
    CREATE,
    EXTRACT,
    LIST
} flag;

long reverse(long x){
     x = ((x & 0xffff0000ffff0000 ) >> 16) 
          | ((x & 0x0000ffff0000ffff ) << 16);
     x = ((x & 0xff00ff00ff00ff00 ) >> 8) 
          | ((x & 0x00ff00ff00ff00ff ) << 8);
     return x;
}


void create(char* filename, char** files, int count) {
    
    FILE* output = fopen(filename,"wb");
    
    FILE* input;

    for (int filesIterator=0;filesIterator<count;filesIterator++) {
        input = fopen(files[filesIterator],"rb");
        if (input == NULL) {
            printf("Error! No such file - %s\n",files[filesIterator]);
            continue;
        }

        unsigned char nameSize = 0;
        while (files[filesIterator][nameSize]) {
            nameSize++;
        }
        
        putc(nameSize,output);

        nameSize=0;
        while (files[filesIterator][nameSize]) {
            putc(files[filesIterator][nameSize],output);
            nameSize++;
        }

        long fileSize;

        fseek(input, 0L, SEEK_END);

        fileSize = ftell(input);

        fseek(input, 0L, SEEK_SET);

        putc(fileSize%256,output);
        putc((fileSize>>8)%256,output);
        putc((fileSize>>16)%256,output);
        putc(fileSize>>24,output);

        int c;
        while ((c = getc(input)) != EOF) {
            putc(c,output);
        }
        fclose(input);
    }
    printf("Array succesfully created");
    fclose(output);
}

void createZipped(char* filename, char** files, int count) {
    // FILE* output = fopen(filename,"wb");
    printf("Here\n");
    FILE* input;
    unsigned int difCounter[255];
    for (int i=0;i<256;i++) {
        difCounter[i] = 0;
    }

    for (int filesIterator=0;filesIterator<count;filesIterator++) {
        input = fopen(files[filesIterator],"rb");
        if (input == NULL) {
            continue;
        }
        int c;
        while ((c=getc(input))!=EOF) {
            difCounter[c]++;
        }
        fclose(input);
    }

    for (int i=0;i<256;i++) {
        if (difCounter[i]!=0) {
            printf("%x - %d\n",i,difCounter[i]);
        }
    }


    // for (int filesIterator=0;filesIterator<count;filesIterator++) {
    //     input = fopen(files[filesIterator],"rb");
    //     if (input == NULL) {
    //         printf("Error! No such file - %s\n",files[filesIterator]);
    //         continue;
    //     }

    //     unsigned char nameSize = 0;
    //     while (files[filesIterator][nameSize]) {
    //         nameSize++;
    //     }
        
    //     putc(nameSize,output);

    //     nameSize=0;
    //     while (files[filesIterator][nameSize]) {
    //         putc(files[filesIterator][nameSize],output);
    //         nameSize++;
    //     }

    //     long fileSize;

    //     fseek(input, 0L, SEEK_END);

    //     fileSize = ftell(input);

    //     fseek(input, 0L, SEEK_SET);

    //     putc(fileSize%256,output);
    //     putc((fileSize>>8)%256,output);
    //     putc((fileSize>>16)%256,output);
    //     putc(fileSize>>24,output);

    //     int c;
    //     while ((c = getc(input)) != EOF) {
    //         putc(c,output);
    //     }
    //     fclose(input);
    // }
    // printf("Array succesfully created");
    // fclose(output);
}

void list(char* filename) {
    FILE* input;
    input = fopen(filename,"rb");
    if (input == NULL) {
        printf("Wrong archive name!\n");
        return;
    }
    
    long fileSize;

    fseek(input, 0L, SEEK_END);
    fileSize = ftell(input);
    fseek(input, 0L, SEEK_SET);

    int size;
    long fsize;
    printf("List of files:\n");
    while (ftell(input)<fileSize) {
        size = fgetc(input);
        for (int i=0;i<size;i++) {
            printf("%c",getc(input));
        }
        printf("\n");
        fsize = getc(input);
        fsize = fsize + (getc(input)<<8);
        fsize = fsize + (getc(input)<<16);
        fsize = fsize + (getc(input)<<24);
        for (long i=0;i<fsize;i++) {
            getc(input);
        }
    }
    fclose(input);
}

void extract(char* filename) {
    FILE* input;
    input = fopen(filename,"rb");
    if (input == NULL) {
        printf("Wrong archive name!\n");
        return;
    }
    
    long fileSize;

    fseek(input, 0L, SEEK_END);
    fileSize = ftell(input);
    fseek(input, 0L, SEEK_SET);

    int size;
    long fsize;
    FILE* output;
    printf("Unpacking files:\n");
    while (ftell(input)<fileSize) {
        size = fgetc(input);
        char outFilename[100];
        fread(outFilename,1,size,input);
        printf("%s\n",outFilename);
        output = fopen(outFilename,"wb");
        fsize = getc(input);
        fsize = fsize + (getc(input)<<8);
        fsize = fsize + (getc(input)<<16);
        fsize = fsize + (getc(input)<<24);
        for (long i=0;i<fsize;i++) {
            putc(getc(input),output);
        }
        fclose(output);
    }
    printf("Finished.");
    fclose(input);
}

int main(int argc, char *argv[]) {
    int compression = 0;
    char** files = (char**)malloc(sizeof(char*)*(argc-3));
    char* filename;
    int count = 0;
    int zipped = 0;

    for (int i=1;i<argc;i++) {
        switch(argv[i][2]) {
            case 'f':
                filename = argv[i+1];
                i++;
                break;
            case 'c':
                i++;
                int k=0;
                while (i<argc && argv[i][0]!='-') {
                    files[k]=argv[i];
                    k++;
                    i++;
                }
                count = k;
                flag = CREATE;
                break;
            case 'e':
                flag = EXTRACT;
                break;
            case 'l':
                flag = LIST;
                break;
            case 'z':
                zipped = 1;
                
        }
    }
    switch (flag) {
        case CREATE:
            if (zipped==0) {
                create(filename,files,count);
            } else {
                createZipped(filename,files,count);
            }
            
            break;
        case LIST:
            list(filename);
            break;
        case EXTRACT:
            extract(filename);
            break;
    }
    free(files);
}