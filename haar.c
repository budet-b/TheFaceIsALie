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

int partition (haarRecord* tab, int l, int h)
{
    int x = tab[h].value;
    int i = (l - 1);
    
    for (int j = l; j <= h- 1; j++)
    {
        if (tab[j].value <= x)
        {
            i++;
            swap (&tab[i], &tab[j]);
        }
    }
    swap (&tab[i + 1], &tab[h]);
    return (i + 1);
}


void quickSort(haarRecord* tab, int l, int h)
{
    if (l < h)
    {
        int p = partition(tab, l, h);
        quickSort(tab, l, p - 1);
        quickSort(tab, p + 1, h);
    }
}

void sort(haarRecord* tab, int NbFeatures) {
    int l = 0;
    int h = NbFeatures - 1;
    quickSort(tab, l, h);
}

void printIntImage(int** integralImage) {
    for(int i = 0; i < 20; i++)
        for(int j = 0; j < 20; j++)
            printf("Integral Image: %d", integralImage[i][j]);
}

haarRecord singleFeature(SDL_Surface *image, int nbFeature) {
    ToGray(image);
    Binarize(image);
    const int imageW = image->w;
    const int imageH = image->h;
	const int frameSize = 24;
	const int features = 5;
	//All five feature types:
    const int feature[5][2] = {{1,2}, {2,1}, {3,1}, {1,3}, {2,2}};
	int** integralImage = matrix_integralImage(image);
    
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
                            SDL_FreeSurface(image);
                            for(int i = 0; i < image->w; i++)
                                free(integralImage[i]);
                            free(integralImage);
                            return(haarOutput);
                        }
                        count = count + 1;
                	}
            	}
            }
    	}
	}
    for(int i = 0; i < image->w; i++)
        free(integralImage[i]);
    free(integralImage);
    SDL_FreeSurface(image);
    return(haarOutput);
}


haarRecord* processImage(SDL_Surface *image, haarRecord* haarOutputTab) {
    ToGray(image);
    Binarize(image);
    const int imageW = image->w;
    const int imageH = image->h;
	const int frameSize = 24;
	const int features = 5;
	//All five feature types:
    const int feature[5][2] = {{1,2}, {2,1}, {3,1}, {1,3}, {2,2}};
	int** integralImage = matrix_integralImage(image);

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
						value = haarProcess(integralImage , x, y, width, height, i+1);
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
    printf("count: %d\n",f);
   //sort(haarOutputTab, *NbFeatures);
    free(image);
    for(int i = 0; i < image->w; i++)
        free(integralImage[i]);
    free(integralImage);
    return haarOutputTab;
}
