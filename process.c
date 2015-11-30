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
    strongClassifier *strong = malloc(sizeof(struct strongClassifier) * 200);
    FILE* file = NULL;
    double result = 0;
    readClassifier(strong, file);

    for(int i = 0; i < 200; i++)
        result = result + strong[i].alpha * (double)applyWeakClassifierMod
            (haarTab,strong[i].classifier);
    free(haarTab);
    return result;
}

int process(char* image) {
    int** integralImage = matrix_integralImage(load_image(image));
    haarRecord* haarTab = malloc(162336 * sizeof(struct haarRecord));
    processImage(integralImage, haarTab);
    double result = applyClassifier(haarTab);
    
    if(result > 0)
        return 1; //FACE
    else
        return 0; //NO FACE
    
    for(int i = 0; i < 24; i++)
        free(integralImage[i]);
    free(integralImage);
}
