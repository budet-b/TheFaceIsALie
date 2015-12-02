//haar.h

#ifndef _haar_h_
#define _haar_h_
struct haarRecord
{
        int haar;
            unsigned long i , j;
                int w,h;
                    int value;
};

typedef struct haarRecord haarRecord;

int calcul_area(int x,int y,int x2,int y2,int** mat);
int feature1(int x,int y,int x2,int y2,int** mat);
int feature2(int x,int y,int x2,int y2,int** mat);
int feature3(int x,int y,int x2,int y2,int** mat);
int feature4(int x,int y,int x2,int y2,int** mat);
int feature5(int x,int y,int x2,int y2,int** mat);
int haarProcess(int** integralImage, int x, int y, int w, int h, int feature);
void swap (haarRecord* a, haarRecord* b);
int partition (haarRecord* tab, int l, int h);
void sort(haarRecord* tab, int NbFeatures);
haarRecord singleFeature(int** integralImage, int nbFeature);
haarRecord* processImage(int** integralImage, haarRecord* haarOutputTab);
int*** getIntegralImages(char* trainingExamples[], int nbExamples);
haarRecord getSingleFeatureOpt(haarRecord blueprint, int** integralImage);
haarRecord* makeSingleFeature(haarRecord blueprint, int*** integralImages, int nbExamples);
#endif
