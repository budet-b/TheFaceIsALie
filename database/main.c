#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void add(int argc,char* argv[],FILE *name){
    if(argc < 5) 
        errx(2, "Insuffisant argument");
    else
        fprintf(name,"%s %s %s\n",argv[2],argv[3],argv[4]);
}
void read(FILE *name) {
    int ch;
    while((ch = fgetc(name)) != EOF)
        printf("%c", (char)ch);
} 

int search(char* argv[],FILE *name) {
    char h[100];
    char result[100];
    int line = 0;
    line++;
    char *input = argv[2];

    while(fgets(h, 100, name) != NULL) {
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
    return line;
}



int main(int argc, char* argv[]) {
    FILE *name;
    
    if((name = (fopen("name", "a+"))) == NULL) {
        fprintf(stderr,"Impossible d'ouvrir le fichier données en lecture\n");
        fclose(name);
    }
    
    if(argc < 2) 
        errx(2, "Insuffisant argument");

    if(strcmp(argv[1], "add") == 0){
        name = fopen("name","a+");
        add(argc, argv,name);
        fclose(name);
    }
    
    if(strcmp(argv[1], "read") == 0) {
        name = fopen("name","r");
        read(name);
        fclose(name);
    }

    if(strcmp(argv[1], "search") == 0) {
        name = fopen("name","r");
        search(argv,name);
        fclose(name);
    }
    
    return 0;
}
