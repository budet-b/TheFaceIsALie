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

struct adaFeature
{
    struct haarRecord f;
    int visage;
    double weight;
};

typedef struct adaFeature adaFeature;


int calcul_area(int x,int y,int x2,int y2,int** mat);
int feature1(int x,int y,int x2,int y2,int** mat);
int feature2(int x,int y,int x2,int y2,int** mat);
int feature3(int x,int y,int x2,int y2,int** mat);
int feature4(int x,int y,int x2,int y2,int** mat);
int feature5(int x,int y,int x2,int y2,int** mat);
int haarProcess(int** integralImage, int x, int y, int w, int h, int feature);
void sort(haarRecord* tab, int NbFeatures, int* visage, double* weights);
haarRecord singleFeature(int** integralImage, int nbFeature);
haarRecord* processImage(int** integralImage, haarRecord* haarOutputTab);
int*** getIntegralImages(char* trainingExamples[], int nbExamples);
haarRecord getSingleFeatureOpt(haarRecord blueprint, int** integralImage);
struct adaFeature* makeSingleFeature(haarRecord blueprint, int*** integralImages, int nbExamples, int* visage, double* weights);
int compare(const void *a, const void *b);
#endif
