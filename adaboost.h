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

struct strongClassifier {
    double alpha;
    struct weakClassifier* classifier;
};

typedef struct strongClassifier strongClassifier;
typedef struct weakClassifier weakClassifier;

int min(haarRecord* haarTab, int nbFeatures);
int max(haarRecord* haarTab, int nbFeatures);
double sum(int* visage, double* weights, int check, int nbFeatures);
void allocate(int* tab);
strongClassifier* adaboost (char* trainingExample[], int* visage, int visagePos, int visageNeg, int trainingRound);

#endif /* adaboost_h */
