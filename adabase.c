#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <err.h>
#include <string.h>
#include <time.h>

#include "image.h"
#include "haar.h"
#include "adaboost.h"

void add(int argc,char* argv[],FILE *name){
    if(argc < 5) 
        errx(2, "Insuffisant argument");
    else
        fprintf(name,"%s %s %s\n",argv[2],argv[3],argv[4]);
}
void readDB(FILE *name) {
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


void writeClassifier(struct strongClassifier *strong) {
    FILE *file = fopen("dataSoutenance","wt");
    struct strongClassifier str;
    struct weakClassifier weak;
    struct haarRecord haar;
    int i = 0;
    str = strong[i];
    while(str.alpha != -1) {
        weak = str.classifier;
        haar = weak.f;
        fprintf(file,"1|%d|%lu|%lu|%d|%d|%d|%d|%d|%f|%f|\r\n",haar.haar,haar.i,haar.j,haar.w,haar.h,haar.value,weak.threshold, weak.toggle, weak.error,str.alpha);
        i++;
        str = strong[i];
    }
    fclose(file);
}



strongClassifier* readClassifier() {
    FILE* file = fopen("data","rt");
    struct strongClassifier *strong =  malloc(200*sizeof(struct strongClassifier));
    struct strongClassifier str;
    struct weakClassifier weak;
    struct haarRecord haar;
    char ch[300];
    char *token;
    int i = 0;
    while(fgets(ch,300,file)!= NULL) {
        char* chr = ch;
        double d;
        int arg = 0;
        while ((token = strsep(&chr, "|")) != NULL) {
            switch (arg) {
            case 1 :
                haar.haar = atoi(token);
                break;
            case 2 : 
                haar.i = atoi(token);
                break;
            case 3 : 
                haar.j = atoi(token);
                break;
            case 4 : 
                haar.w = atoi(token);
                break;
            case 5 : 
                haar.h = atoi(token);
                break;
            case 6 : 
                haar.value = atoi(token);
                break;
            case 7 : 
                weak.threshold = atoi(token);
                break;
            case 8 : 
                weak.toggle = atoi(token);
                break;
            case 9 : 
                sscanf(token, "%lf", &d);
                weak.error = d;
                break;
            case 10 :
                sscanf(token, "%lf", &d);
                str.alpha = d;
                break;
            default :
                break;
            }
            arg++;
        }
        weak.f = haar;
        str.classifier = weak;
        strong[i] = str;
        i++;
    }
    fclose(file);
    return strong;
}
