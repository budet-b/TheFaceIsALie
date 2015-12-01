//
//  haar.c
//  
//
//  Created by TheFaceIsALie on 10/10/2015.
//
//

#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <err.h>

#include "haar.h"
#include "image.h"
#include "adaboost.h"
#include "adabase.h"

int calcul_area(int x,int y,int x2,int y2,int** mat) {
    int a = mat[x][y];
    //i,j
    int b = mat[x][y2];
    //i,j-1
    int c = mat[x2][y];
    //i-1;j
    int d = mat[x2][y2];
    //i-1;j-1
    //printf("%d + %d - %d - %d\n",a,d,b,c);
    return (a + d - b - c);
}
//Rectangle : Noir à gauche, blanc droite
int feature1(int x,int y, int h, int w, int** mat) {
    int a = calcul_area(x, y, x - h, y - (w / 2), mat);
    int b = calcul_area(x, y - (w / 2), x - h, y - w, mat);
    //printf("%d - %d\n", b, a);
    return (b - a); // a - b
}
//Rectangle : Noir en bas, blanc en haut
int feature2(int x, int y, int h, int w, int** mat) {
    int a = calcul_area(x, y, x-(h/2), y-w, mat);
    int b = calcul_area(x-(h/2), y, x-h, y-w, mat);
    return (b - a);
}
//Rectangle : Noir au mileu, blanc des deux autres cotés VERTICAL
int feature3(int x, int y, int h, int w, int** mat) {
    int a = calcul_area(x, y, x-h, y-(w/3),mat);
    int b = calcul_area(x, y-((2*w)/3), x-h, y-w, mat);
    int c = calcul_area(x, y-(w/3), x-h, y-((2*w)/3), mat);
    return (a + b - c); // c - a - b
}
//Rectangle : Noir au mileu, blanc des deux autres cotés HORIZONTAL
int feature4(int x, int y, int h, int w, int** mat) {
    int a = calcul_area(x, y, x-(h/3), y-w,mat);
    int b = calcul_area(x-(h/3), y, x-((2*h)/3), y-w, mat);
    int c = calcul_area(x-((2*h)/3), y, x-h, y-w, mat);
    return (a + c - b); // b - a -c
}

int feature5(int x, int y, int h, int w, int** mat) {
    int a = calcul_area(x, y, (x-h)/2, y - (w/2),mat);
    int b = calcul_area(x, y - (w/2), x - (h/2), y - w, mat);
    int c = calcul_area(x - (h/2), y, x - h, y - (w/2), mat);
    int d = calcul_area(x - (h/2), y - (w/2), x - h, y - w ,mat);
    return (a + d - b -c); // b + c - a - d
}

int haarProcess(int** integralImage, int x, int y, int w, int h, int feature) {
    //printf("feature: %d\n", feature);    
    switch (feature) {
            case 1:
                return feature1(x, y, w, h, integralImage);
                break;
            
            case 2:
                return feature2(x, y, w, h, integralImage);
                break;
                
            case 3:
                return feature3(x, y, w, h, integralImage);
                break;
                
            case 4:
                return feature4(x, y, w, h, integralImage);
                break;
                
            case 5:
                return feature5(x , y , w, h, integralImage);
                break;

            default:
                return 0;
                break;
        }
}




void swap (haarRecord* a, haarRecord* b)
{
    haarRecord temp = *a;
    *a = *b;
    *b = temp;
}

void swapD(double* a, double* b) {
    double temp = *a;
    *a = *b;
    *b = temp;
}

void swapI(int* a, int* b) {
    int temp = *a;
    *a = *a;
    *b = temp;
}

int partition (haarRecord* tab, int l, int h, int* visage, double* weights)
{
    int x = tab[h].value;
    int i = (l - 1);
    
    for (int j = l; j <= h- 1; j++)
    {
        if (tab[j].value <= x)
        {
            i++;
            swap (&tab[i], &tab[j]);
            swapD(&weights[i], &weights[j]);
            swapI(&visage[i], &visage[j]);
        }
    }
    swap (&tab[i + 1], &tab[h]);
    swapD(&weights[i+1], &weights[h]);
    swapI(&visage[i+1], &visage[h]);
    return (i + 1);
}


void quickSort(haarRecord* tab, int l, int h, int* visage, double* weights)
{
    if (l < h)
    {
        int p = partition(tab, l, h, visage, weights);
        quickSort(tab, l, p - 1, visage, weights);
        quickSort(tab, p + 1, h, visage, weights);
    }
}
 
void sort(haarRecord* tab, int NbFeatures, int* visage, double* weights) {
    int l = 0;
    int h = NbFeatures - 1;
    quickSort(tab, l, h, visage, weights);
}


void printIntImage(int** integralImage) {
    for(int i = 0; i < 20; i++)
        for(int j = 0; j < 20; j++)
            printf("Integral Image: %d", integralImage[i][j]);
}

haarRecord singleFeature(int** integralImage, int nbFeature) {
    const int imageW = 24;
    const int imageH = 24;
	const int frameSize = 24;
	const int features = 5;
	//All five feature types:
    const int feature[5][2] = {{1,2}, {2,1}, {3,1}, {1,3}, {2,2}};
	//int** integralImage = matrix_integralImage(image);
    
	int count = 0;
	int value;
	haarRecord haarOutput;

	//Each feature:
	for (int i = 0; i < features; i++) {
    	int sizeX = feature[i][0];
    	int sizeY = feature[i][1];
    	
        //Each detection zone within framesize posible
        for (int width = sizeX - 1; width < frameSize; width+=sizeX){
        	for (int height = sizeY - 1; height < frameSize; height+=sizeY){
            	//Each position fiting in framesize with current detection zone
            	for (int x = width; x < imageW; x++) {
                	for (int y = height; y < imageH; y++) {
                        if(count == nbFeature) {
						    value = haarProcess(integralImage , x, y, width, height, i+1);
                            //printf("RECORDING: %d\n", value);
                            haarOutput.value = value;
                            haarOutput.haar = i + 1;
                            haarOutput.i = x;
                            haarOutput.j = y;
                            haarOutput.w = width;
                            haarOutput.h = height;
                            return(haarOutput);
                        }
                        count = count + 1;
                	}
            	}
            }
    	}
	}
    return(haarOutput);
}


haarRecord* processImage(int** integralImages, haarRecord* haarOutputTab) {
    const int imageW = 24;
    const int imageH = 24;
	const int frameSize = 24;
	const int features = 5;
	//All five feature types:
const int feature[5][2] = {{1,2}, {2,1}, {3,1}, {1,3}, {2,2}};
	//int** integralImage = matrix_integralImage(image);
        
	int f = 0;
	int value;
	haarRecord haarOutput;

	//Each feature:
	for (int i = 0; i < features; i++) {
    	int sizeX = feature[i][0];
    	int sizeY = feature[i][1];
    	
        //Each detection zone within framesize posible
        for (int width = sizeX - 1; width < frameSize; width+=sizeX){
        	for (int height = sizeY - 1; height < frameSize; height+=sizeY){
            	//Each position fiting in framesize with current detection zone
            	for (int x = width; x < imageW; x++) {
                	for (int y = height; y < imageH; y++) {
						value = haarProcess(integralImages, x, y, width, height, i+1);
                        haarOutput.value = value;
                        haarOutput.haar = i + 1;
                        haarOutput.i = x;
                        haarOutput.j = y;
                        haarOutput.w = width;
                        haarOutput.h = height;
                        haarOutputTab[f] = haarOutput;
                        f = f + 1;
                	}
            	}
            }
    	}
	}
    return haarOutputTab;
}

int*** getIntegralImages(char* trainingExamples[], int nbExamples) {
    SDL_Surface** image_array = load_image_array(trainingExamples, nbExamples);
    int*** integralImages = malloc(nbExamples * sizeof(int*));

    for(int i = 0; i < nbExamples; i++) { 
        ToGray(image_array[i]);
        integralImages[i] = matrix_integralImage(image_array[i]);
        SDL_FreeSurface(image_array[i]);
    }
    free(image_array);
    return integralImages;
}

haarRecord getSingleFeatureOpt(haarRecord blueprint, int** integralImage) {
    int value = haarProcess(integralImage, blueprint.i, blueprint.j, blueprint.w, blueprint.h, blueprint.haar);
    haarRecord haarOutput;
    haarOutput.value = value;
    haarOutput.i = blueprint.i;
    haarOutput.j = blueprint.j;
    haarOutput.w = blueprint.w;
    haarOutput.h = blueprint.h;
    haarOutput.haar = blueprint.haar;
    return haarOutput;
}

haarRecord* makeSingleFeature(haarRecord blueprint,int*** integralImages, int nbExamples) {
    haarRecord* haarOutput = malloc(nbExamples*sizeof(haarRecord));
    for(int i = 0; i < nbExamples; i++) {
        haarOutput[i] = getSingleFeatureOpt(blueprint, integralImages[i]);
    }
    return haarOutput;
}
