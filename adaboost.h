//  adaboost.h
//  
//
//  Created by Benjamin Budet on 16/10/2015.
//
//

#ifndef adaboost_h
#define adaboost_h
#include "image.h"
#include "haar.h"

struct weakClassifier {
    struct haarRecord f;
    int threshold;
    int toggle;
    double error;
    int margin;
};

typedef struct weakClassifier weakClassifier;

int min(haarRecord* haarTab, int nbFeatures);
int max(haarRecord* haarTab, int nbFeatures);
double sum(int* visage, double* weights, int check, int nbFeatures);
void allocate(int* tab);
haarRecord** processMultipleImages(char* trainingExamples[], int nbExamples);
weakClassifier* decisionStump (haarRecord *haarTab, int* visage, double* weights, int nbExamples);
weakClassifier* bestStump (haarRecord** haarTab, int* visage, double* weights, int nbFeatures);
void adaboost (char* trainingExample[], int* visage, int visagePos, int visageNeg, int trainingRound);

#endif /* adaboost_h */
