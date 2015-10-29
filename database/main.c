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

    if(strcmp(argv[1], "read") == 0) {
        while((ch = fgetc(name)) != EOF)
            printf("%c", (char)ch);
    }

    if(strcmp(argv[1], "search") == 0) {
        char *input = argv[2];
        int begin = 0;
        while((fgets(h, 100, name) != NULL) && result == NULL) {
            for(int i = 0; strcmp(&h[i],&input[i]) == 0;i++) {
                printf("Test %d",i);
                if(strcmp(&h[i]," ") == 0) {   
                    begin = i;
                    break;
                }
            }
            line++;
            size_t len = strlen(h);
            /*for(int i = begin; i < (int)len; i++)
              result[i] = h[i];*/
            strcpy(result,h);
            printf("ça marche : %s Ligne : %d\n",result ,line);
        }
    }
    fclose(name);
    return 0;
}
