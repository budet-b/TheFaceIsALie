//
//  adaboost.c
//  
//
//  Created by TheFaceIsALie on 16/10/2015.
//
//
#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <err.h>
#include <math.h>

#include "image.h"
#include "haar.h"
#include "adaboost.h"

//TO DO : linear combination for strong classifier


int min(haarRecord* haarTab, int nbFeatures) {
    int minHaarTab = haarTab[0].value;
    for(int i = 1; i < nbFeatures; i++)
        if(haarTab[i].value > minHaarTab)
            minHaarTab = haarTab[i].value;
    return minHaarTab;
}

int max(haarRecord* haarTab, int nbFeatures) {
    int maxHaarTab = haarTab[0].value;
    for(int i = 1; i < nbFeatures; i++)
        if(haarTab[i].value < maxHaarTab)
            maxHaarTab = haarTab[i].value;
    return maxHaarTab;
}

double sum(int* visage, double* weights, int check, int nbFeatures) {
    int sum = 0;
    for(int i = 0; i < nbFeatures; i++) {
        if (check == visage[i]) {
            sum = sum + weights[i];
        }
    }
    return sum;
}

double* weightInit(double* weights, int* visage, int nbExamplesNeg, int nbExamplesPos) {
    weights = malloc((nbExamplesPos + nbExamplesNeg) * sizeof(weakClassifier));
    for(int i = 0; i < (nbExamplesPos + nbExamplesNeg); i++) {
        if(visage[i] == -1)
            weights[i] = (1/(2*nbExamplesNeg));
        else
            weights[i] = (1/(2*nbExamplesPos));
    }
    return weights;
}

int compareHaar(haarRecord haarTab, weakClassifier DS) {
    return(haarTab.w == DS.f.w && haarTab.h == DS.f.h && haarTab.i == DS.f.i && haarTab.j == DS.f.j && haarTab.haar == DS.f.haar);
}

int applyWeakClassifier(weakClassifier* DS, char* image) {
    int nbFeatures;
    haarRecord* haarTab = processImage(load_image(image), &nbFeatures);
    for(int i = 0; ; i++){
        if(compareHaar(haarTab[i], *DS)) { 
            if(DS->toggle == 1 && haarTab[i].value > DS->threshold) 
                return DS->toggle;
            else
                return -1;
        }
    }
}

void updateWeights(char* trainingExamples[], weakClassifier* DS, double* weights, int nbExamples) {
    int visage;
    for(int i = 0; i < nbExamples; i++) {
        visage = applyWeakClassifier(DS,trainingExamples[i]);
        if(visage == -1)
            weights[i] = (weights[i]/2) * (1/DS->error);
        else
            weights[i] = (weights[i]/2) * (1/(1 - DS->error));
    }
}

haarRecord** processMultipleImages(char* trainingExamples[], int nbExamples) {
    SDL_Surface** image_array = load_image_array(trainingExamples, nbExamples);
    haarRecord** haarTmp = NULL;
    haarRecord** haarOutput = NULL;
    haarOutput = malloc(162336*sizeof(void*));
    for(int i = 0; i < nbExamples; i++)
        haarOutput[i] = malloc(nbExamples*sizeof(haarRecord));

    int nbFeatures; // not necessary, always 162336

    for(int i = 0; i < nbExamples; i++)//get haarTab for each image 
        haarTmp[i] = processImage(image_array[i], &nbFeatures);
    
    
    for(int i = 0; i < 162336; i++) { // MEMORY EATER
        for(int j = 0; j < nbExamples; j++) {
            haarOutput[i][j] = haarTmp[j][i];
        }
    }
    for(int i = 0; i < 162336; i++)
        sort(haarOutput[i], 162336); // sort each row, also 2nd MEMORY EATER
    
    free(haarTmp);
    return haarOutput;
}


weakClassifier* decisionStump (haarRecord *haarTab, int* visage, double* weights, int nbExamples){

    //temp = current
    //notemp = previous
    
    //margin init
    int margin = 0;
    int marginTemp = margin;
    
    //threshold init
    int threshold = haarTab[0].value - 1;
    int thresholdTemp = threshold;
    
    //error init
    double error = 2; // Arbitrary upper bound
    double errorTemp;
    
    //toggle init
    int toggle;
    int toggleTemp;

    //sumWeights init
    double WPosBig = sum(visage, weights, nbExamples, 1);
    double WNegBig = sum(visage, weights, nbExamples, -1);
    double WPosSmall = 0;
    double WNegSmall = 0;
    
    //var init
    int j = 0;
    double errorPos;
    double errorNeg;
    
    while (1) {
        errorPos = WPosSmall + WNegBig;
        errorNeg = WNegSmall + WPosBig;
        if (errorPos < errorNeg) {
            errorTemp = errorPos;
            toggleTemp = 1;
        }
        else {
            errorTemp = errorNeg;
            toggleTemp = -1;
        }
        if ((errorTemp < error) || ((errorTemp == error) && (marginTemp > margin ))) {
            error = errorTemp;
            toggle = toggleTemp;
            margin = marginTemp;
            threshold = thresholdTemp;
        }
        if (j == nbExamples) {
            break;
        }
        while (1) {
            if (visage[j] == -1) { //updating weights balance with current treshold
                WNegSmall = WNegSmall + weights[j];
                WNegBig = WNegBig - weights[j];
            }
            else {
                WPosSmall = WPosSmall + weights[j];
                WPosBig = WPosBig - weights[j];
            }
            if (j == nbExamples || haarTab[j].value != haarTab[j+1].value) { //find new valid treshold
                break;
            }
            else {
                j++;
            }
        }
        if (j == nbExamples) {
            thresholdTemp = haarTab[nbExamples - 1].value + 1;
            marginTemp = 0;
        }
        else {
            thresholdTemp = (haarTab[j].value + haarTab[j+1].value) / 2;
            marginTemp = haarTab[j+1].value - haarTab[j].value;
        }
    }
    weakClassifier* bestWeak = NULL;
    bestWeak->threshold = threshold;
    bestWeak->toggle = toggle;
    bestWeak->error = error;
    bestWeak->margin = margin;
    return bestWeak;
}

weakClassifier* bestStump (haarRecord** haarFeatures, int* visage, double* weights, int nbExamples){
    weakClassifier* currentDS;
    weakClassifier* bestDS = NULL;
    bestDS->threshold = 0;
    bestDS->toggle = 0;
    bestDS->error = 2;
    bestDS->margin = 0;
    for (int f = 0; f < 162336; f++) {
        currentDS = decisionStump(haarFeatures[f], visage, weights, nbExamples);
        currentDS->f = *haarFeatures[f];
         if ((currentDS->error < bestDS->error) || ((currentDS->error == bestDS->error) && (currentDS->margin > bestDS->margin)))
            bestDS = currentDS;
    }
    return bestDS;
}

//Numbers of images < nb training Round
void adaboost (char* trainingExamples[], int* visage, int visagePos, int visageNeg, int trainingRound){
    int nbExamples = visagePos + visageNeg;
    double* weights = NULL;
    double alpha;
    struct haarRecord** haarFeatures;
    struct weakClassifier *currentDS = NULL;
    weightInit(weights, visage, visagePos, visageNeg);
    haarFeatures = processMultipleImages(trainingExamples, nbExamples);

    for (int i = 0; i < trainingRound; i++) {
        currentDS = bestStump(haarFeatures, visage, weights, nbExamples);
        if(currentDS->error == 0 && i == 0)
            return currentDS;
        else {
            alpha = (1/2)*log((1 - currentDS->error)/currentDS->error);
            updateweights(trainingExamples, currentDS, weights, nbExamples); 
        }
    }

}
