//
//  adaboost.h
//  
//
//  Created by Benjamin Budet on 16/10/2015.
//
//

#ifndef adaboost_h
#define adaboost_h


int min(haarRecord* haarTab, int nbFeatures);
int max(haarRecord* haarTab, int nbFeatures);
int sumBig(haarRecord* haarTab, int nbFeatures,int threshold, int check);
int sumSmall(haarRecord* haarTab, int threshold, int nbFeatures, int check);
void allocate(int* tab);
int* decisionStump (haarRecord haarFeature, haarRecord *haarTab, int nbFeatures, int visage);
int* bestStump (haarRecord *haarTab, int nbFeatures, int visage);
void adaboost (char* trainingExample[], int* visage, int trainingRound);

#endif /* adaboost_h */
