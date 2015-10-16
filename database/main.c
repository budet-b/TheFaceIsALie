#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]) {
    FILE *name;
    name = fopen ("name", "a+");
    int ch;

    /*if( argc != 4 )
        errx(2, "Insuffisant argument");*/
    if( strcmp(argv[1], "add") == 0) {
        fprintf(name, argv[2] );
        fprintf(name, " ");
        fprintf(name, argv[3] );
        // rajouter les carac de la personne
        fprintf(name, "\n");
    }

    if( strcmp(argv[1], "read") == 0) {
        while((ch = fgetc(name)) != EOF )
            printf("%c", (char)ch);
    }

    if( strcmp(argv[1], "search") == 0) {
        while((ch = fgetc(name)) != EOF) {
            if(strcmp(argv[2], ch) == 0)
                printf("ça marche");
        }
    }

    fclose(name);
    return 0;
}
