#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <err.h>

#include "image.h"
#include "haar.h"
#include "adaboost.h"


SDL_Surface* load_image(char *path) {
    SDL_Surface  *image;
    printf("%s\n", path);
    image = IMG_Load(path);
    if (!image) {
        errx(3, "Unable to load image at : %s : %s", path,IMG_GetError());
    }
    return image;
}

SDL_Surface** load_image_array(char *path[], int nbExamples) {
    SDL_Surface **image_array = malloc(nbExamples * sizeof(SDL_Surface*));
    SDL_Surface *image;
    for(int i = 0; i < nbExamples; i++) {
        image = IMG_Load(path[i]);
        if (!image) {
            errx(3, "Unable to load image at : %s : %s", path[i],IMG_GetError());
        }
        image_array[i] = image;
    }
    return image_array;
}

  static
Uint8* pixelref(SDL_Surface *image, unsigned x, unsigned y) 
{
  int pix = image->format->BytesPerPixel;
  return (Uint8*)image->pixels + y * image->pitch + x * pix;
}


void putpixel(SDL_Surface *image, unsigned x, unsigned y, Uint32 pixel) {
  Uint8 *p = pixelref(image, x, y);
  switch(image->format->BytesPerPixel) {
  case 1:
    *p = pixel;
    break;
  case 2:
    *(Uint16 *)p = pixel;
    break;
  case 3:
    if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
      p[0] = (pixel >> 16) & 0xff;
      p[1] = (pixel >> 8) & 0xff;
      p[2] = pixel & 0xff;
    } else {
      p[0] = pixel & 0xff;
      p[1] = (pixel >> 8) & 0xff;
      p[2] = (pixel >> 16) & 0xff;
    }
    break;
  case 4:
    *(Uint32 *)p = pixel;
    break;
  }
}

Uint32 getpixel(SDL_Surface *image, unsigned x, unsigned y) {
    Uint8 *p = pixelref(image, x, y);
  switch(image->format->BytesPerPixel) {
    case 1:
      return *p;
    case 2:
      return *(Uint16 *)p;
    case 3: //Compatibility for processor types (BIG_ENDIAN for PowerPC/ARM ... | LIL_ENDIAN for x86-64 ...)
      if(SDL_BYTEORDER == SDL_BIG_ENDIAN) 
        return p[0] << 16 | p[1] << 8 | p[2];
      else
        return p[0] | p[1] << 8 | p[2] << 16;
    case 4:
      return *(Uint32 *)p;
  }
  return 0;
}


void ToGray(SDL_Surface *image) {
    SDL_LockSurface(image);
    Uint8 r = 0;
    Uint8 g = 0;
    Uint8 b = 0;
    Uint32 graypixel;
    for(int i = 0; i < image->w; i++) { //Column route
        for(int j = 0; j < image->h; j++){
            Uint32 pixel =  getpixel(image,i,j);
            SDL_GetRGB(pixel, image->format, &r, &g, &b);
            r = r * 0.3 + g * 0.53 + b * 0.11;
            graypixel = SDL_MapRGB(image->format, r, r, r);
           // printf("%d\n",graypixel);
           // printf(" r = %d\n",r);
            putpixel(image, i, j, graypixel);
        }
    }
    SDL_UnlockSurface(image);
}

void Binarize(SDL_Surface *surface)
{
  SDL_LockSurface(surface);
  Uint8 r=0;
  Uint8 g=0;
  Uint8 b=0;
  Uint32 pink;
  for(int i =0; i < surface->w; i++)
  {
    for (int j=0; j < surface->h; j++)
    {
      Uint32 pixel = getpixel(surface,i,j);
      SDL_GetRGB(pixel, surface->format,&r,&g,&b);
      if (r > 125)
      {
        r =255;
      }
      else
      {
        r = 0;
      }
      pink = SDL_MapRGB(surface->format,r,r,r);
     // printf("%d\n",pink);
      putpixel(surface,i,j,pink);
    }
  }
  SDL_UnlockSurface(surface);
}

SDL_Surface integralImage(SDL_Surface *image) {
    SDL_Surface *outImage = SDL_CreateRGBSurface(0, image->w, image->h, 32, 0, 0, 0, 0);
    ToGray(outImage);
    Binarize(outImage);
    Uint32 pixel = getpixel(image,0,0);
    putpixel(outImage,0,0,pixel);
    SDL_LockSurface(image);
    SDL_LockSurface(outImage);
    for(int i = 1; i < image->w; i++){
        printf("%d\n",i); 
        pixel = getpixel(image,i,0)+getpixel(outImage,i-1,0);
        putpixel(outImage,i,0,pixel);
    }
    for(int i = 1; i < image->h; i++){
        printf("%d\n",i);
        pixel = getpixel(image,0,i)+getpixel(outImage,0,i-1);
        putpixel(outImage,0,i,pixel);
    } 
    for(int i = 1; i < image->w; i++)
    {
        for(int j = 1; j < image->h; j++)
    	{
            printf("%d,%d\n",i,j);
           pixel = getpixel(image,i,j) + getpixel(outImage,i,j-1) + getpixel(image,i-1,j) - getpixel(outImage,i-1,j-1);
           putpixel(outImage,i,j,pixel);
        }
    }
    SDL_UnlockSurface(outImage);
    SDL_UnlockSurface(image);
    return *outImage;
}


// MATRIX SECTION

void display_matrix(SDL_Surface* image, int** mat) {
    for(int i = 0; i < image->w; i++){
        for(int j = 0; j < image->h; j++) {
            printf("| %d |", mat[i][j]);
        }
        printf("\n");
    }
    
}
int** build_matrix_image(SDL_Surface *image) {
   int** matrix; 
   matrix = malloc(image->w * sizeof(int*));
   for(int i = 0; i<image->w; i++) {
      matrix[i] = malloc(image->h * sizeof(int *));
     }
  return matrix;
}


int** matrix_integralImageL(int** mat, SDL_Surface *image){
    for(int i = 1; i < image->w; i++){
        Uint8 r=0;
        Uint8 g=0;
        Uint8 b=0;
        Uint32 pixel = getpixel(image,i,0);
        SDL_GetRGB(pixel, image ->format, &r, &g, &b);
        mat[i][0] = r + mat[i-1][0];
        //mat[i][0] = getpixel(image,i,0);
        //printf("%d\n",mat[i][0]);
    }   
    return mat;
}


int** matrix_integralImageC(int** mat, SDL_Surface *image){
    for(int i = 1; i < image->h; i++){
        Uint8 r=0;
        Uint8 g=0;
        Uint8 b=0;
        Uint32 pixel = getpixel(image,0,i);
        SDL_GetRGB(pixel, image ->format, &r, &g, &b);
        mat[0][i] = r + mat[0][i-1];
        //printf("%d\n",mat[0][i]);
        //mat[0][i] = getpixel(image,0,i);

    } 
    return mat;
}


int** matrix_integralImageT(int** mat, SDL_Surface *image){
    for(int i = 1; i < image->w; i++){
        for(int j = 1; j < image->h; j++) {
            Uint8 r=0;
            Uint8 g=0;
            Uint8 b=0;
            Uint32 pixel = getpixel(image,i,j);
            SDL_GetRGB(pixel, image ->format, &r, &g, &b);
            mat[i][j] = r + mat[i-1][j] + mat[i][j-1] - mat[i-1][j-1];
            //mat[i][j] = getpixel(image,i,j) + mat[i-1][j] + mat[i][j-1] - mat[i-1][j-1];
        }
    } 
    return mat;
}

int** matrix_integralImage(SDL_Surface *image){
    SDL_LockSurface(image);
    int** matrix = build_matrix_image(image);
    matrix[0][0] = 255;
    matrix_integralImageL(matrix,image);
    matrix_integralImageC(matrix,image);
    matrix_integralImageT(matrix,image);
    SDL_UnlockSurface(image);
    return matrix;
}
