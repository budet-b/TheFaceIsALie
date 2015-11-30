#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <err.h>

#include "haar.h"
#include "image.h"
#include "adaboost.h"
#include "process.h"
#include "adabase.h"

/*
void writeClassifier(struct strongClassifier *strong) {
    FILE *file = fopen("data","wt");
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



void readClassifier(struct strongClassifier *strong) {
    FILE* file = fopen("data","rt");
    struct strongClassifier str;
    struct weakClassifier weak;
    struct haarRecord haar;
    char ch[300];
    int i = 0;
    while(strong[i].alpha != -1) {
        printf("Reading %d: %f\n", i, strong[i].alpha);
        fgets(ch,200,file);
        sscanf(ch,"%d %lu %lu %d %d %d | %d %d %lf %d | %lf |\n",&(haar.haar),&(haar.i),&(haar.j),&(haar.w),&(haar.h),&(haar.value),&(weak.threshold),&(weak.toggle),&(weak.error),&(weak.margin),&(str.alpha));
        weak.f = haar;
        str.classifier = weak;
        strong[i] = str;
        i++;
    }
    fclose(file);
}
*/

int compareHaarMod(haarRecord haarTab, weakClassifier DS) {
    if(haarTab.w == DS.f.w && haarTab.h == DS.f.h && haarTab.i == DS.f.i && haarTab.j == DS.f.j && haarTab.haar == DS.f.haar)
        return 1;
    return 0;
}

int applyWeakClassifierMod(haarRecord* haarTab, weakClassifier classifier) {
    for(int i = 0; i < 162336; i++){
        if(compareHaarMod(haarTab[i], classifier)) {
            if(haarTab[i].value > classifier.threshold) {
                return 1;
            }
            else {
                return -1;
            }
        }
    }
    return 1;
}

double applyClassifier(haarRecord* haarTab) {
    struct strongClassifier *strong; 
    strong = malloc(3 * sizeof(struct strongClassifier));
    double result = 0;
    printf("Reading\n");
    readClassifier(strong);
    printf("Starting Applying\n");

    for(int i = 0; i < 5; i++) {
        result = result + strong[i].alpha * (double)applyWeakClassifierMod(haarTab,strong[i].classifier);
        printf("Reading %d\n", i);
    }
        free(haarTab);
    return result;
}

int process(char* image) {
    int** integralImage = matrix_integralImage(load_image(image));
    haarRecord* haarTab = malloc(162336 * sizeof(struct haarRecord));
    processImage(integralImage, haarTab);
    double result = applyClassifier(haarTab);
        
    for(int i = 0; i < 24; i++)
        free(integralImage[i]);
    free(integralImage);

    if(result > 0)
        return 1; //FACE
    else
        return 0; //NO FACE
}
