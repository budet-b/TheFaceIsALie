//
//  adaboost.c
//  
//
//  Created by TheFaceIsALie on 16/10/2015.
//
//
//#include "image.h"
//#include <math.h>
//#include "adaboost.h"

void decisionStump (haarRecord *haarTab, int n, int visage, int threshold, int toggle, int error, int Margin){
    
    //margin init
    Margin = 0;
    int MarginTemp = Margin;
    
    //threshold init
    threshold = min(haarTab);
    int thresholdTemp = threshold;
    
    //error init
    error = 2;
    int errorTemp;
    
    //toggle init
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
        if (j == n) {
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
            if (j == n) {
                break;
            }
            else {
                j++;
            }
        }
        if (j == n) {
            thresholdTemp = max(haarTab)+1;
            MarginTemp = 0;
        }
        else {
            thresholdTemp = (haarTab[j] + haarTab[j+1]) / 2;
            MarginTemp = haarTab[j+1] - haarTab[j];
        }
    }
    int[4] decisionStump = {threshold, toggle, error, Margin};
    return decisionStump;
}

int[] bestStump (haarRecord *haarTab, int nbFeatures, int visage){
    int bestError = 2;
    int threshold,toggle,error,margin;
    int[4] currentDecisionStump;
    int[4] bestDecisionStump = {0, 0, 2, 0}
    for (int f = 0; O < nbFeatures; f++) {
        currentDecisionStump = decisionStump(haarTab, nbFeatures, visage, &threshold, &toggle, &error, &margin);
         if ((errorTemp < bestDecisionStump[3]) || ((errorTemp == bestDecisionStump[3])) && (MarginTemp > bestDecisionStump[4]))
            bestDecisionStump = currentDecisionStump;
    }
    return bestDecisionStump;
}

//Numbers of images < nb training Round
void adaboost (char* trainingExample[], int* checksum, int trainingRound){
    int weight = 1;
    int nbFeatures;
    int error;
    int[4] currentDecisionStump;
    for (int i = 0; i < trainingRound; i++) {
        currentDecisionStump = bestStump(processImage(load_image(trainingExample[i]), &nbFeatures),&nbFeatures, checksum[i]);
    }
}
