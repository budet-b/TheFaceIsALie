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
    double sum = 0;
    for(int i = 0; i < nbFeatures; i++) {
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
    weights = malloc((nbExamplesPos + nbExamplesNeg) * sizeof(double));
    for(int i = 0; i < (nbExamplesPos + nbExamplesNeg); i++) {
        printf("assigning weight to example %d\n",i);
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

int applyWeakClassifier(weakClassifier* DS, haarRecord* haarTab) {
    int nbFeatures;
    for(int i = 0; i < 162336; i++){
        if(compareHaar(haarTab[i], *DS)) {
            printf("Comparison %d > %d\n",haarTab[i].value, DS->threshold);
            if(haarTab[i].value > DS->threshold) 
                return DS->toggle;
            else
                return -1*(DS->toggle);
        }
    }
    free(haarTab);
    return 1;
}

double calWeightedError(haarRecord** haarNM, double* weights, int* visage, weakClassifier* DS, int nbExamples) {
    double weightedError = 0;
    for(int i = 0; i < nbExamples; i++) {
        if(applyWeakClassifier(DS, haarNM[i]) != visage[i])
            weightedError += weights[i];
    }
    return weightedError;
}
void updateWeights(haarRecord** haarNM, weakClassifier* DS,int* visage, double* weights, int nbExamples) {
    int checksum;
    for(int i = 0; i < nbExamples; i++) {
        printf("applying weak classifier to example %d\n",i);
        checksum = applyWeakClassifier(DS,haarNM[i]);
        if(checksum != visage[i]) {
            weights[i] = weights[i]*1.5;
            printf("Classifier is wrong, %d : %d\n", checksum, visage[i]);
        }
        else{
            weights[i] = weights[i]/1.5; // (1.0 - weightedError)
            printf("Classifier is right, %d : %d\n", checksum, visage[i]);
        }
    }
}

void display_weights(double* weights, int* visage, int nbExamples) {
    for(int i = 0; i < nbExamples; i++)
        printf("weights: %f, visage: %d\n",weights[i], visage[i]); 
}

haarRecord** processMultipleImages(char* trainingExamples[], int nbExamples, int check) {
    SDL_Surface** image_array = load_image_array(trainingExamples, nbExamples);
    haarRecord** haarTmp = malloc(nbExamples*sizeof(void*));
    haarRecord** haarOutput = malloc(162336*sizeof(void*));
    for(int i = 0; i < nbExamples; i++){
        haarTmp[i] = malloc(162336*sizeof(haarRecord));
    }
    for(int i = 0; i < 162336; i++) {
        haarOutput[i] = malloc(nbExamples*sizeof(haarRecord));
    }

    int nbFeatures; // not necessary, always 162336

    for(int i = 0; i < nbExamples; i++){//get haarTab for each image
        printf("getting haarfeatures for image %d\n",i);
        haarTmp[i] = processImage(image_array[i], &nbFeatures);
    }
    if(check){
        free(haarOutput);
        for(int i = 0; i < nbExamples; i++)
            //sort(haarTmp[i], 162335);
        return haarTmp;
    }
    for(int i = 0; i < nbExamples; i++) { // MEMORY EATER
        for(int j = 0; j < 162336; j++) {
            //printf("reversing %d,%d <- %d,%d\n",j,i,i,j);
            haarOutput[j][i] = haarTmp[i][j];
        }
    }
    for(int i = 0; i < 162336; i++){
        //printf("sorting row %d\n",i);
        sort(haarOutput[i], nbExamples); // sort each row, also 2nd MEMORY EATER
    }
    free(haarTmp);
    return haarOutput;
}

void write(strongClassifier *strongTab, int nbClassifier) {
    FILE* data;
    data = fopen("data.bin", "w+b");
    if(data == NULL)
        errx(2, "Couldn't open data.bin");
    //fwrite(strongTab, sizeof(struct strongClassifier) * nbClassifier, 1, data);
    for(int i = 0; i < nbClassifier; i++) {
        printf("Weak Classifier nb %d\n", i);
        printf("\t Alpha = %f\n", strongTab[i].alpha);
        printf("\t Threshold = %d\n", strongTab[i].classifier->threshold);
    }

    fwrite(strongTab, sizeof(struct strongClassifier), nbClassifier, data);
    fclose(data);
}

void read(int nbClassifier) {
    FILE* data;
    data = fopen("data.bin", "w+b");
    if(data == NULL)
        errx(2, "Couldn't open data.bin");
    
    strongClassifier* strongTab;
    strongTab = malloc(nbClassifier * sizeof(struct strongClassifier));
    fread(strongTab, sizeof(struct strongClassifier), nbClassifier, data);

    for(int i = 0; i < nbClassifier; i++) {
        printf("Weak Classifier nb %d\n", i);
        printf("\t Alpha = %f\n", strongTab->alpha);
        printf("\t Threshold = %d\n", strongTab->classifier->threshold);
    }
    fclose(data);
}

weakClassifier* decisionStump (haarRecord *haarTab, int* visage, double* weights, int nbExamples){

    //temp = current
    //notemp = previous
    
    haarRecord haarTmp;
    
    //margin init
    int margin = 0;
    int marginTemp = margin;
    
    //threshold init
    int threshold = haarTab[0].value;
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
            haarTmp = haarTab[j];
            //printf("new treshold found %d\n", threshold);
        }
        if (j == nbExamples - 1) {
            break;
        }
        j++;
        while (1) {
            if (visage[j] == -1) { //updating weights balance with current treshold
                WNegSmall = WNegSmall + weights[j];
                WNegBig = WNegBig - weights[j];
            }
            else {
                WPosSmall = WPosSmall + weights[j];
                WPosBig = WPosBig - weights[j];
            }
            if (j == nbExamples - 1 || haarTab[j].value != haarTab[j+1].value) { //find new valid treshold
                break;
            }
            else {
                j++;
            }
        }
        if (j == nbExamples - 1) {
            thresholdTemp = haarTab[nbExamples - 1].value + 1;
            marginTemp = 0;
        }
        else {
            thresholdTemp = haarTab[j+1].value;
            marginTemp = haarTab[j+1].value - haarTab[j].value;
        }
    }
    weakClassifier* bestWeak = malloc(sizeof(struct weakClassifier));
    bestWeak->f = haarTmp;
    bestWeak->threshold = threshold;
    bestWeak->toggle = toggle;
    bestWeak->error = error;
    bestWeak->margin = margin;
    return bestWeak;
}

weakClassifier* bestStump (haarRecord** haarFeatures, int* visage, double* weights, int nbExamples){
    weakClassifier* currentDS;
    weakClassifier *bestDS = malloc(sizeof(struct weakClassifier));
    bestDS->threshold = 0;
    bestDS->toggle = 0;
    bestDS->error = 2;
    bestDS->margin = 0;
    for (int f = 0; f < 162336; f++) {
        //printf("Working on feature %d\n",f);
        currentDS = decisionStump(haarFeatures[f], visage, weights, nbExamples);
        //printf("CurrentDS:\n\t error: %d \n\t threshold: %d \n", currentDS->error, currentDS->threshold);
        if ((currentDS->error < bestDS->error) || ((currentDS->error == bestDS->error) && (currentDS->margin > bestDS->margin))) {
            bestDS = currentDS;
            //free(currentDS);
        }
    }
    free(currentDS);
    return bestDS;
}

//Numbers of images < nb training Round
strongClassifier* adaboost (char* trainingExamples[], int* visage, int visagePos, int visageNeg, int trainingRound){
    strongClassifier* result = NULL;
    result = malloc(trainingRound * sizeof(strongClassifier));
    int nbExamples = visagePos + visageNeg;
    double* weights = NULL;
    double alpha;
    double weightedError;
    struct haarRecord** haarFeatures;
    struct haarRecord** haarNM;
    struct weakClassifier *currentDS = NULL;
    weights = weightInit(weights, visage, visagePos, visageNeg);
    haarFeatures = processMultipleImages(trainingExamples, nbExamples, 0);
    haarNM = processMultipleImages(trainingExamples, nbExamples, 1);

    for (int i = 0; i < trainingRound; i++) {
        printf("size of haarFeatures: %d\n", sizeof(haarFeatures));
        printf("size of haarNM: %d\n", sizeof(haarNM));
        display_weights(weights, visage, nbExamples);
        printf("round %d\n",i);
        currentDS = bestStump(haarFeatures, visage, weights, nbExamples);
        printf("processing alpha\n");
        weightedError = calWeightedError(haarNM, weights, visage, currentDS, nbExamples);
        printf("Weighted Error %f\n", weightedError);
        alpha = 0.5*log((1.0 - weightedError)/weightedError);
        updateWeights(haarNM, currentDS, visage, weights, nbExamples);
        weights = normalizeWeights(weights, nbExamples);
        printf("adding weak classifier\n");
        result[i].alpha = alpha;
        result[i].classifier = currentDS;
        free(currentDS);
    }
    free(haarFeatures);
    free(haarNM);
    write(result, trainingRound);
    read(trainingRound);
    free(result);
    return result;
}
