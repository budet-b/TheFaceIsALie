//
//  adaboost.c
//  
//
//  Created by TheFaceIsALie on 16/10/2015.
//
//
//#include "image.h"
//#include <math.h>
#include "haar.c"

int min(haarRecord* haarTab, int nbFeatures) {
    int minHaarTab = 0;
    for(int i = 0; i < nbFeatures; i++)
        if(haarTab[i].value > minHaarTab)
            minHaarTab = haarTab[i].value;
    return minHaarTab;
}

int max(haarRecord* haarTab, int nbFeatures) {
    int maxHaarTab = 0;
    for(int i = 0; i < nbFeatures; i++)
        if(haarTab[i].value < maxHaarTab)
            maxHaarTab = haarTab[i].value;
    return maxHaarTab;
}


int* decisionStump (haarRecord *haarTab, int nbFeatures, int visage){
    
    //margin init
    int Margin = 0;
    int MarginTemp = Margin;
    
    //threshold init
    int threshold = min(haarTab, nbFeatures);
    int thresholdTemp = threshold;
    
    //error init
    int error = 2;
    int errorTemp;
    
    //toggle init
    int toggle;
    int toggleTemp;

    //weights init
    int WPosBig = sumBig(haarTab, 1); //int 1 or -1, 1 = pos; -1 = neg
    int WNegBig = sumBig(haarTab, -1);
    int WPosSmall = sumSmall(haarTab, 1);
    int WNegSmall = sumSmall(haarTab, -1);
    
    //var init
    int j = 0;
    int errorPos;
    int errorNeg;
    
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
        if ((errorTemp < error) || ((errorTemp == error) && (MarginTemp > Margin ))) {
            error = errorTemp;
            toggle = toggleTemp;
            Margin = MarginTemp;
            threshold = thresholdTemp;
        }
        if (j == nbFeatures) {
            break;
        }
        while (1) {
            if (visage == -1) {
                WNegSmall = WNegSmall + haarTab[j].weight;
                WNegBig = WNegBig - haarTab[j].weight;
            }
            else {
                WPosSmall = WPosSmall + haarTab[j].weight;
                WPosBig = WPosBig - haarTab[j].weight;
            }
            if (j == nbFeatures) {
                break;
            }
            else {
                j++;
            }
        }
        if (j == nbFeatures) {
            thresholdTemp = max(haarTab, nbFeatures)+1;
            MarginTemp = 0;
        }
        else {
            thresholdTemp = (haarTab[j].value + haarTab[j+1].value) / 2;
            MarginTemp = haarTab[j+1].value - haarTab[j].value;
        }
    }
    int decisionStump[4] = {threshold, toggle, error, Margin};
    return *decisionStump;
}

int* bestStump (haarRecord *haarTab, int nbFeatures, int visage){
    int bestError = 2;
    //threshold,toggle,error,margin;
    int currentDS[4];
    int bestDS[4] = {0, 0, 2, 0};
    for (int f = 0; f < nbFeatures; f++) {
        *currentDS = *decisionStump(haarTab, nbFeatures, visage);
         if ((currentDS[2] < bestDS[2]) || ((currentDS[2] == bestDS[2]) && (currentDS[3] > bestDS[3])))
            *bestDS = *currentDS;
    }
    return *bestDS;
}

//Numbers of images < nb training Round
void adaboost (char* trainingExample[], int* visage, int trainingRound){
    int weight = 1;
    int nbFeatures;
    int error;
    int currentDS[4];
    for (int i = 0; i < trainingRound; i++) {
        *currentDS = bestStump(processImage(load_image(trainingExample[i]), &nbFeatures),&nbFeatures, visage[i]);
    }
}
