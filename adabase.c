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


void writeClassifier(struct strongClassifier *strong,FILE *file) {
    file = fopen("data.bin","wt");
    struct strongClassifier str;
    struct weakClassifier weak;
    struct haarRecord haar;
    int i = 0;
    while(str.alpha != -1) {
        str = strong[i];
        weak = str.classifier;
        haar = weak.f;
        fprintf(file,"%d %lu %lu %d %d %d | %d %d %f %d | %f |\r\n",haar.haar,haar.i,haar.j,haar.w,haar.h,haar.value,weak.threshold, weak.toggle, weak.error, weak.margin,str.alpha);
        i++;
    }
    fclose(file);
}


void readClassifier(struct strongClassifier* strong,FILE *file) {
    file = fopen("data.bin","rt");
    struct strongClassifier str;
    struct weakClassifier weak;
    struct haarRecord haar;
    char ch[300];
    int i = 0;
    while(strong[i].alpha != -1) {
        fgets(ch,200,file);
        sscanf(ch,"%d %lu %lu %d %d %d | %d %d %lf %d | %lf |\n",&(haar.haar),&(haar.i),&(haar.j),&(haar.w),&(haar.h),&(haar.value),&(weak.threshold),&(weak.toggle),&(weak.error),&(weak.margin),&(str.alpha));
        weak.f = haar;
        str.classifier = weak;
        strong[i] = str;
        i++;
    }
    fclose(file);
}
