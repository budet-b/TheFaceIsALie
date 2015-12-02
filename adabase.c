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


void writeClassifier(struct strongClassifier *strong) {
    FILE *file = fopen("data","wt");
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
                //printf("%d ",haar.haar);
                break;
            case 2 : 
                haar.i = atoi(token);
                //printf("%lu ",haar.i);

                break;
            case 3 : 
                haar.j = atoi(token);
                //printf("%lu ",haar.j);
                break;
            case 4 : 
                haar.w = atoi(token);
                //printf("%d ",haar.w);
                break;
            case 5 : 
                haar.h = atoi(token);
                //printf("%d ",haar.h);
                break;
            case 6 : 
                haar.value = atoi(token);
                //printf("%d ",haar.value);
                break;
            case 7 : 
                weak.threshold = atoi(token);
                //printf("%d ",weak.threshold);
                break;
            case 8 : 
                weak.toggle = atoi(token);
                //printf("%d ",weak.toggle);
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
        //printf("Reading %d: %f\n", i, strong[i].alpha);
        i++;
    }
    fclose(file);
    return strong;
}


/*int main(int argc, char* argv[]) {    
    struct haarRecord ha;
    ha.haar = ha.w = ha.h = ha.value = 0;
    ha.i = ha.j = 2;
    
    struct weakClassifier weak;
    weak.f = ha;
    weak.threshold = weak.toggle = 1;
    weak.error = 2;

    struct strongClassifier s;
    s.alpha = 4;
    s.classifier = weak;

    struct strongClassifier s2;
    s2.alpha = 3;
    s2.classifier = weak;

    struct strongClassifier s4;
    s4.alpha = 1;
    s4.classifier = weak;

    struct strongClassifier s5;
    s5.alpha = 3;
    s5.classifier = weak;

    struct strongClassifier s6;
    s6.alpha = 1;
    s6.classifier = weak;

    struct strongClassifier s7;
    s7.alpha = 3;
    s7.classifier = weak;

    struct strongClassifier s8;
    s8.alpha = -1;
    s8.classifier = weak;


    struct strongClassifier s3;
    s3.alpha = 1;
    s3.classifier = weak;

    struct strongClassifier *str = malloc(8*sizeof(struct strongClassifier));
    str[0] = s;
    str[1] = s2;
    str[2] = s3;
    str[3] = s4;
    str[4] = s5;
    str[5] = s6;
    str[6] = s7;
    str[7] = s8;

    struct strongClassifier *strong = malloc(5*sizeof(struct strongClassifier)); 

    if(argc < 2) 
        errx(2, "Insuffisant argument");

    if(strcmp(argv[1], "write") == 0) {
        writeClassifier(str);
        free(str);
    }
    
    if(strcmp(argv[1], "read") == 0) {
        strong = readClassifier();
        for(int i = 0; i < 5; i++)
        printf("classifier i:%d j:%d w:%d h:%d haar:%d\n",strong[i].classifier.f.i, strong[i].classifier.f.j, strong[i].classifier.f.w, strong[i].classifier.f.h, strong[i].classifier.f.haar);
    }
    return 0;
}*/
