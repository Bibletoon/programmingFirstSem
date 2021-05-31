#include <stdio.h>

int main(int argc, char* argv[]) {
    if (argc < 4) {
        printf("Error! Not enought arguments!");
        return -1;
    } 
    FILE* in;
    in = fopen(argv[1], "r");
    if (in == NULL) {
        printf("Error! Wrong first file name!");
        printf(argv[1]);
        return -1;
    }
    FILE* out;
    out = fopen(argv[3],"w");
    char sym = fgetc(in);
    while (sym != EOF) {
        fputc(sym,out);
        sym = fgetc(in);
    }
    fclose(in);
    in = fopen(argv[2], "r");
    if (in == NULL) {
        printf("Error! Wrong second file name!");
        return -1;
    }
    sym = fgetc(in);
    while (sym != EOF) {
        fputc(sym,out);
        sym = fgetc(in);
    }
    fclose(in);
    fclose(out);
}