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
    char result[100];

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
        while((fgets(h, 100, name) != NULL) && result[0] == '\0') {
            char *space = strchr(h, ' ');
            char *dynh = h;
            size_t length = space - dynh;
            strncpy(result,dynh,length);
            
            if(strcmp(result,input)!=0)
                memset(result, '\0',100);
            else {
                printf("Result : ");
                for(space++;*space != '\0';space++)
                    printf("%c",*space);
                break;
            }
        }
        if(result[0] == '\0') {
            printf("Unrecognised haar feature : ");
            printf("%s\n",argv[2]);   
        }
    }
    fclose(name);
    return 0;
}
