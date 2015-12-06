//image.h

#ifndef _image_h_
#define _image_h_
#include "adaboost.h"
#include "haar.h"


SDL_Surface* load_image(char *path);
SDL_Surface** load_image_array(char *path[], int nbExamples);

    static inline
Uint8* pixelref(SDL_Surface *image, unsigned x, unsigned y);
void putpixel(SDL_Surface *image, unsigned x, unsigned y, Uint32 pixel) ; 
Uint32 getpixel(SDL_Surface *image, unsigned x, unsigned y);

void ToGray(SDL_Surface *surface);
void Binarize(SDL_Surface *surface);

SDL_Surface integralImage(SDL_Surface *image);

//Matrix

void display_matrix(SDL_Surface* image, int** mat);
int** build_matrix_image(SDL_Surface *image);
int** matrix_integralImageL(int** mat, SDL_Surface *image);
int** matrix_integralImageC(int** mat, SDL_Surface *image);
int** matrix_integralImage(SDL_Surface *image);
char* copy_filename(char* image);
int copy_file(char* old_filename, char *new_filename);
void drawRect(haarRecord* haarTab, SDL_Surface *ecran);

#endif



