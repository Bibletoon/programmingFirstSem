#include <stdio.h>
#include <malloc.h>


enum tagFlag {
    CREATE,
    EXTRACT,
    LIST,
    COMPRESS
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
    fputc(0,output);
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

void createRLE(char* filename, char** files, int count) {
    FILE* output = fopen(filename,"wb");
    fputc(1,output);
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

        FILE* compressed = fopen("tmp","wb");
        int c;
        int mode = -1;
        unsigned counter = 0;
        unsigned char buf;
        unsigned char buffer[100000];
        while ((c=fgetc(input))!=EOF) {
            if (mode==-1 && counter == 0) {
                buffer[counter] = c;
                counter++;
            } else if (mode==-1 && counter!=0) {
                if (buffer[0]==c) {
                    mode = 1;
                } else {
                    mode = 0;
                    buffer[counter] = c;
                }
                counter++;
            } else if (mode==0) {
                if (c==buffer[counter-1]) {
                    mode = 1;
                    buf = buffer[counter-1];
                    counter-=1;
                    putc((0<<7)+((counter)>>8),compressed);
                    putc(counter%256,compressed);
                    for (int i=0;i<counter;i++) {
                        fputc(buffer[i],compressed);
                    }
                    buffer[0] = c;
                    counter = 2;
                } else {
                    buffer[counter] = c;
                    counter++;
                }
            } else {
                if (c==buffer[0]) {
                    counter++;
                } else {
                    mode = 0;
                    putc((1<<7)+((counter)>>8),compressed);
                    putc(counter%256,compressed);
                    putc(buffer[0],compressed);
                    buffer[0] = c;
                    counter = 1;
                }
            }
        }
        if (mode == 0) {
            putc((0<<7)+((counter)>>8),compressed);
            putc(counter%256,compressed);
            for (int i=0;i<counter;i++) {
                fputc(buffer[i],compressed);
            }
        } else {
            putc((1<<7)+((counter)>>8),compressed);
            putc(counter%256,compressed);
            putc(buffer[0],compressed);
        }
        fclose(compressed);
        compressed = fopen("tmp","rb");
        long fileSize;
        fseek(compressed,0L,SEEK_END);
        fileSize = ftell(compressed);
        fseek(compressed,0L,SEEK_SET);

        putc(fileSize%256,output);
        putc((fileSize>>8)%256,output);
        putc((fileSize>>16)%256,output);
        putc(fileSize>>24,output);
        
        while ((c=fgetc(compressed))!=EOF) {
            fputc(c,output);
        }
        fclose(compressed);
        fclose(input);
    }
    printf("Array succesfully created");
    fclose(output);
    remove("tmp");
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
    fseek(input, 1, SEEK_SET);

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
    int mode  = getc(input);
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
        if (mode==0) {
                for (long i=0;i<fsize;i++) {
                putc(getc(input),output);
            }
        } else {
            int counter = 0;
            while (counter<fsize) {
                int c = getc(input);
                int submode = c>>7;
                int size = c%128+getc(input);
                counter+=2;
                if (submode==0) {
                    for (int i=0;i<size;i++) {
                        putc(getc(input),output);
                        counter++;
                    }
                } else {
                    c = getc(input);
                    counter++;
                    for (int i=0;i<size;i++) {
                        putc(c,output);
                    }
                }
            }
        }
        fclose(output);
    }
    printf("Finished.");
    fclose(input);
}

int main(int argc, char *argv[]) {
    char** files = (char**)malloc(sizeof(char*)*(argc-3));
    char* filename;
    int count = 0;

    for (int i=1;i<argc;i++) {
        switch(argv[i][2]) {
            case 'f':
                filename = argv[i+1];
                i++;
                break;
            case 'c':
                if (argv[i][3]=='o') {
                    flag=COMPRESS;
                } else {
                i++;
                int k=0;
                while (i<argc && argv[i][0]!='-') {
                    files[k]=argv[i];
                    k++;
                    i++;
                }
                count = k;
                if (flag!=COMPRESS) {
                    flag = CREATE;
                }
                }
                break;
            case 'e':
                flag = EXTRACT;
                break;
            case 'l':
                flag = LIST;
                break;
        }
    }
    switch (flag) {
        case CREATE:
            create(filename,files,count);
            break;
        case LIST:
            list(filename);
            break;
        case EXTRACT:
            extract(filename);
            break;
        case COMPRESS:
            createRLE(filename,files,count);
    }
    free(files);
}