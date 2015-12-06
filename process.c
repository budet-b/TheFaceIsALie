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

int compareHaarMod(haarRecord haarTab, weakClassifier DS) {
    if(haarTab.w == DS.f.w && haarTab.h == DS.f.h && haarTab.i == DS.f.i && haarTab.j == DS.f.j && haarTab.haar == DS.f.haar)
        return 1;
    return 0;
}

int applyWeakClassifierMod(haarRecord* haarTab, weakClassifier classifier) {
        //printf("classifier i:%d j:%d w:%d h:%d haar:%d\n",classifier.f.i, classifier.f.j, classifier.f.w, classifier.f.h, classifier.f.haar);
    for(int i = 0; i < 162336; i++) {
        if(compareHaarMod(haarTab[i], classifier)) {
            if(classifier.toggle == 1) {
                printf("Said %d >= %d\n", haarTab[i].value, classifier.threshold);
                return (haarTab[i].value >= classifier.threshold)?1:-1;
            }
            else {
                printf("Said %d <= %d\n", haarTab[i].value, classifier.threshold);
                return (haarTab[i].value <= classifier.threshold)?1:-1;
            }
        }
    }
    printf("SAID ADADADA\n");
    return 1;
}

double applyClassifier(haarRecord* haarTab) {
    struct strongClassifier *strong; 
    double result = 0;
    printf("Reading...\n");
    strong = readClassifier();
    printf("Starting Applying\n");

    for(int i = 0; i < 10; i++) {
        result = result + strong[i].alpha * (double)applyWeakClassifierMod(haarTab,strong[i].classifier);
        printf("Reading %d ==> Result: %f\n", i, result);
    }
        free(haarTab);
    return result;
}

haarRecord* haarReturn() {
    struct strongClassifier *strong = readClassifier();
    haarRecord* haarTab = malloc(200*sizeof(haarRecord));
    for(int i = 0; i<10;i++) {
        haarTab[i] = strong[i].classifier.f;
        if(i == 9)
            haarTab[i+1].haar = -1;
        printf("ada{%d}",i);
    }
    free(strong);
    return haarTab;
}


struct haarRecord* process(char* image) {
    int** integralImage = matrix_integralImage(load_image(image));
    haarRecord* haarTab = malloc(162336 * sizeof(struct haarRecord));
    processImage(integralImage, haarTab);
    double result = applyClassifier(haarTab);
    for(int i = 0; i < 24; i++)
        free(integralImage[i]);
    free(integralImage);
    haarRecord* haarOutput = haarReturn();
    if(result > 0) {
        printf("Face detected \n");
        return haarOutput; //FACE
    }
    else {
        printf("No Face detected \n");
        return haarOutput; //NO FACE
    }
}
