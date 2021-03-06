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
#include <time.h>


#include "image.h"
#include "haar.h"
#include "adaboost.h"
#include "adabase.h"
//TO DO : opti apply weak classifier

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

double sum(int check, adaFeature* haarTab, int threshold) {
    double sum = 0;
    for(int i = 0; haarTab[i].f.value > threshold; i++) {
        if (check == *haarTab[i].visage) {
            sum = sum + *haarTab[i].weight;
        }
    }
    return sum;
}

double sumSmall(int* visage, double* weights, int check, haarRecord* haarTab, int threshold) {
    double sum = 0;
    for(int i = 0; haarTab[i].value < threshold; i++) {
        if (check == visage[i]) {
            sum = sum + weights[i];
        }
    }
    return sum;
}

double sumWeight(double* weights, int nbExamples) {
    double sum = 0;
    for(int i = 0; i < nbExamples; i++)
        sum = sum + weights[i];
    return sum;
}
double* normalizeWeights(double* weights, int nbExamples) {
    double sum = sumWeight(weights, nbExamples);
    for(int i = 0; i < nbExamples; i++)
        weights[i] = weights[i]/sum;
    return weights;
}

double* weightInit(double* weights, int* visage, int nbExamplesNeg, int nbExamplesPos) {
    for(int i = 0; i < (nbExamplesPos + nbExamplesNeg); i++) {
        if(visage[i] == -1)
            weights[i] = (0.5*(double)nbExamplesNeg);
        else
            weights[i] = (0.5*(double)nbExamplesPos);
    }
    weights = normalizeWeights(weights, nbExamplesNeg + nbExamplesPos);
    return weights;
}

int compareHaar(haarRecord haarTab, weakClassifier DS) {
    if(haarTab.w == DS.f.w && haarTab.h == DS.f.h && haarTab.i == DS.f.i && haarTab.j == DS.f.j && haarTab.haar == DS.f.haar)
        return 1;
    return 0;
}

int applyWeakClassifier(haarRecord* haarTab, weakClassifier* DS) {
    for(int i = 0; i < 162336; i++){
        if(compareHaar(haarTab[i], *DS)) {
            if(DS->toggle == 1) {
                return (haarTab[i].value >= DS->threshold)?1:-1;
            }
            else {
                return (haarTab[i].value >= DS->threshold)?-1:1;
            }
        }
    }
    free(haarTab);
    return 1;
}

double calWeightedError(int*** integralImages, double* weights, int* visage, weakClassifier* DS, int nbExamples) {
    double error = 0;
    double right = 0;
    haarRecord *haarTab;
    for(int i = 0; i < nbExamples; i++) {
        haarTab = malloc(162336 * sizeof(haarRecord));
        processImage(integralImages[i], haarTab);
        if(applyWeakClassifier(haarTab, DS) != visage[i])
            error += weights[i];
        else 
            right += weights[i];
        free(haarTab);
    }
    return((double)0.5 - (double)0.5*(error));
}

void updateWeights(int*** integralImages, weakClassifier* DS,int* visage, double* weights, int nbExamples, double wError) {
    haarRecord* haarTab;
    for(int i = 0; i < nbExamples; i++) {
        haarTab = malloc(162336 * sizeof(haarRecord));
        processImage(integralImages[i], haarTab);
        if(applyWeakClassifier(haarTab, DS) != visage[i]) 
            weights[i] = (weights[i] / (double)2) * ((double)1 / wError);
        else 
            weights[i] = (weights[i]/ (double)2) * ((double)1 / (1 - wError));
        free(haarTab);
    }
}

void display_weights(double* weights, int* visage, int nbExamples) {
    for(int i = 0; i < nbExamples; i++)
        printf("weights: %f, visage: %d\n",weights[i], visage[i]); 
}

haarRecord* processSingleFeature(int*** integralImages, int nbExamples,  int nbFeature) {
    haarRecord* haarFeature = malloc(nbExamples*sizeof(haarRecord));
    
    for(int i = 0; i < nbExamples; i++)
        haarFeature[i] = singleFeature(integralImages[i], nbFeature);
    return haarFeature;
}

weakClassifier* decisionStump (adaFeature *haarTab, int nbExamples){    
    haarRecord haarTmp;
    
    //threshold init
    int threshold = haarTab[0].f.value;
    int thresholdTemp = threshold;
    
    //error init
    double error = 2; // Arbitrary upper bound
    double errorTemp;
    
    //toggle init
    int toggle;
    int toggleTemp;


    //sumWeights init 
    double WPosBig = sum(1, haarTab, thresholdTemp);
    double WNegBig = sum(-1,  haarTab, thresholdTemp);
    double WPosSmall = 0;
    double WNegSmall = 0;

    //var init
    int j = 0;
    double errorPos;
    double errorNeg;
    
    while(1){
        errorPos = WPosSmall + WNegBig;
        errorNeg = WPosBig + WNegSmall;
        if (errorPos < errorNeg) {
            errorTemp = errorPos;
            toggleTemp = 1;
        }
        else {
            errorTemp = errorNeg;
            toggleTemp = -1;
        }
        if (errorTemp != (double)0 && (errorTemp <= error)) {
            error = errorTemp;
            toggle = toggleTemp;
            threshold = thresholdTemp;
            haarTmp = haarTab[j].f;
        }
        if(j == nbExamples -2)
            break;
        j++;
        while (1) {
            if (*haarTab[j].visage == -1) { //updating weights balance wih current treshold
                WNegSmall = WNegSmall + *haarTab[j].weight;
                WNegBig = WNegBig - *haarTab[j].weight;
            }
            else {
                WPosSmall = WPosSmall + *haarTab[j].weight;
                WPosBig = WPosBig - *haarTab[j].weight;
            }
            if (j == nbExamples - 2 || haarTab[j].f.value != haarTab[j+1].f.value) { //find new valid treshold
                break;
            }
            else {
                j++;
            }
        }
        if (j == nbExamples - 2) {
            thresholdTemp = haarTab[nbExamples - 1].f.value + 1;
        }
        else {
            thresholdTemp = (haarTab[j].f.value + haarTab[j + 1].f.value) / 2;
        }
    }
    weakClassifier* bestWeak = malloc(sizeof(struct weakClassifier));
    bestWeak->f = haarTmp;
    bestWeak->threshold = threshold;
    bestWeak->toggle = toggle;
    bestWeak->error = error;
    return bestWeak;
}

weakClassifier* bestStump (int*** integralImages, int* visage, double* weights, int nbExamples, haarRecord* blueprint){
    weakClassifier *currentDS;
    weakClassifier *bestDS = malloc(sizeof(struct weakClassifier));
    int* visageTemp = visage;
    double* weightsTemp = weights;
    bestDS->threshold = 0;
    bestDS->toggle = 0;
    bestDS->error = 200000;
    adaFeature* haarFeature;
    time_t t1 = time(NULL);
    for (int f = 0; f < 162336; f++) {
        haarFeature = makeSingleFeature(blueprint[f], integralImages, nbExamples, visageTemp, weightsTemp);
        qsort(haarFeature, nbExamples, sizeof(adaFeature), compare); 
        currentDS = decisionStump(haarFeature, nbExamples);
        if (currentDS->error != (double)0 && currentDS->error < bestDS->error) {
            bestDS->f = currentDS->f;
            bestDS->threshold = currentDS->threshold;
            bestDS->toggle = currentDS->toggle;
            bestDS->error = currentDS->error;
        }
        free(haarFeature);
        free(currentDS);
    }
    printf("Exec in %d s\n", (int) (time(NULL) -t1));
    return bestDS;
}

void displayClassifier(weakClassifier class) {
    printf("\thaar: %d\n\ti: %lu\n\tj: %lu\n\tw: %d\n\th: %d\n\tvalue: %d\n",class.f.haar, class.f.i, class.f.j, class.f.w, class.f.h, class.f.value);
    printf("\tthreshold: %d\n\ttoggle: %d\n\terror: %f\n",class.threshold, class.toggle, class.error);
}

strongClassifier* adaboost (char* trainingExamples[], int* visage, int visagePos, int visageNeg, int trainingRound){
    printf("starting adaboost \n");
    strongClassifier* result = malloc((trainingRound + 1)* sizeof(strongClassifier));
    int nbExamples = visagePos + visageNeg;
    double* weights = malloc(nbExamples* sizeof(double));
    double beta;
    double weightedError;
    struct weakClassifier *currentDS;
    double temp;
    weights = weightInit(weights, visage, visagePos, visageNeg);
    int*** integralImages = getIntegralImages(trainingExamples, nbExamples);
    haarRecord* blueprint = malloc(162336 * sizeof(haarRecord));
    processImage(integralImages[0],blueprint);
    printf("init weight : OK \n");
    
    for (int i = 0; i < trainingRound; i++) {
        printf("Round %d\n",i);
        printf("\tWeight normalize\n");
        weights = normalizeWeights(weights, nbExamples);
        currentDS = bestStump(integralImages, visage, weights, nbExamples, blueprint);
        printf("\tProcessing alpha\n");
        weightedError = calWeightedError(integralImages, weights, visage, currentDS, nbExamples);
        printf("\tWeighted Error %f\n", weightedError);
        beta = weightedError/(1 - weightedError);
        printf("\tWeights update\n");
        updateWeights(integralImages, currentDS, visage, weights, nbExamples, weightedError);
        printf("\tAdding weak classifier\n");
        temp = log(1/beta);
        if(isfinite(temp))
            result[i].alpha = temp;
        else
            result[i].alpha = (double)50;
        result[i].classifier.f = currentDS->f;
        result[i].classifier.threshold = currentDS->threshold;
        result[i].classifier.error = currentDS->error;
        result[i].classifier.toggle = currentDS->toggle;
        if(i == trainingRound - 1) {
            result[i + 1].alpha = -1;
            result[i + 1].classifier.f = currentDS->f;
            result[i + 1].classifier.threshold = 0;
            result[i + 1].classifier.error = 0;
            result[i + 1].classifier.toggle = 0;
        } 
        printf("Strong Classifier %d\n\tAlpha: %f\n", i, result[i].alpha);
        displayClassifier(result[i].classifier);
    }
    for(int i = 0; i < trainingRound + 1; i++)
        printf("Alpha[%d] = %f\n",i,result[i].alpha);
    free(weights);
    return result;
}
