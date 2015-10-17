#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]) {
    FILE *name;
    if((name = fopen("name", "r")) == NULL)
        fprintf(stderr,"Impossible d'ouvrir le fichier données en lecture\n");
    int ch;
    char h[100];
    char *result = NULL;
    int line = 0;

    if(argc < 2)
        errx(2, "Insuffisant argument");
    if(strcmp(argv[1], "add") == 0) {
        fprintf(name,"%s",argv[2]);
        fprintf(name, " ");
        fprintf(name,"%s", argv[3]);
        // rajouter les carac de la personne
        fprintf(name, "\n");
    }

    if( strcmp(argv[1], "read") == 0) {
        while((ch = fgetc(name)) != EOF)
            printf("%c", (char)ch);
    }

    if( strcmp(argv[1], "search") == 0) {
        while((fgets(h, 100, name) != NULL) && result == NULL) {
            line++;
            result = strstr(h, argv[2]);
            printf("ça marche : %s Ligne : %d\n",result ,line);
        }
    }
    fclose(name);
    return 0;
}
