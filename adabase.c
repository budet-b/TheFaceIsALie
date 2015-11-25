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
    file = fopen("data.bin","r");
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


int main(int argc, char* argv[]) {
    FILE *file = NULL;
    
    struct haarRecord ha;
    ha.haar = ha.w = ha.h = ha.value = 1;
    ha.i = ha.j = 2;
    
    struct weakClassifier weak;
    weak.f = ha;
    weak.threshold = weak.toggle = weak.margin = 1;
    weak.error = 2;

    struct strongClassifier s;
    s.alpha = 1;
    s.classifier = weak;

    struct strongClassifier s2;
    s2.alpha = 3;
    s2.classifier = weak;

    struct strongClassifier s3;
    s3.alpha = -1;
    s3.classifier = weak;

    struct strongClassifier *str = malloc(3*sizeof(struct strongClassifier));
    str[0] = s;
    str[1] = s2;
    str[2] = s3;

    if(argc < 2) 
        errx(2, "Insuffisant argument");

    if(strcmp(argv[1], "write") == 0) {
        writeClassifier(str,file);
        free(str);
    }
    
    if(strcmp(argv[1], "read") == 0) {
        free(str);
        str = malloc(3*sizeof(struct strongClassifier));
        readClassifier(str,file);
    }
    return 0;
}
