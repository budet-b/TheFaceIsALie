#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <err.h>

#include "haar.h"
#include "image.c"
#include "adaboost.h"
#include "process.h"
#include "adabase.h"

void pause();

char* copy_filename(char* image) {
    int size = strlen(image);
    char* newimage = malloc(5+sizeof(image));
    for(int i = 0; i < size-4; i++) 
        newimage[i] = image[i];
    strcat(newimage,"Copy");
    for(int i = size;i < size+5;i++) {
        newimage[i] = image[i-4];
    }
    return newimage;
}


int copy_file(char* old_filename, char *new_filename) {
	FILE  *ptr_old, *ptr_new;
	int  a;
	ptr_old = fopen(old_filename, "rb");
	ptr_new = fopen(new_filename, "wb");
	while(1){
		a  =  fgetc(ptr_old);
		if(!feof(ptr_old))
			fputc(a, ptr_new);
		else
            break;
    }
	fclose(ptr_new);
    fclose(ptr_old);
	return  0;
}


/*void drawRect(haarRecord* haarTab, char* image) {
    char* imagecopy = copy_filename(image);
    printf("New file = %s\n",imagecopy);
    copy_file(image,imagecopy);
    SDL_Surface *img = load_image(imagecopy);
    SDL_LockSurface(img);
    Uint32 pixel = SDL_MapRGBA(img->format, 127, 255, 0, 255);
    printf("%d\n",(int)sizeof(haarTab));
    for(size_t i = 0;i<3;i++) {
        haarRecord haar = haarTab[i];
        printf("i: %zu\n",i);
        for(int l = 0;l<haar.w;l++) {
            putpixel(img, haar.i - l, haar.j, pixel);
            printf("pixel1");
        }
        for(int l = 0;l<haar.h;l++) {
            putpixel(img, haar.i, haar.j - l, pixel);
            printf("pixel2");
        }
        for(int l = 0;l<haar.h;l++) {
            putpixel(img, haar.i + haar.w, haar.j - l, pixel);
            printf("pixel3");
        }
       for(int l = 0;l<haar.w;l++) {
            putpixel(img, haar.i - l, haar.j + haar.h, pixel);
            printf("pixel4");
        }
    }
    SDL_UnlockSurface(img);
}*/


void drawRect(haarRecord* haarTab, SDL_Surface *ecran) {
    for(size_t i = 0;i<3;i++) {
        haarRecord haar = haarTab[i];
        printf("i: %zu\n",i);
       //Uint32 pixel = SDL_MapRGBA(ecran->format, 127, 255, 0, 255);
       /* for(int l = 0;l<haar.w;l++) {
            *((unsigned int *)ecran->pixels + haar.i +l + haar.j) = 0x00FF00FF;
            printf("pixel1");
        }
        for(int l = 0;l<haar.h;l++) {
            //putpixel(ecran, haar.i, haar.j - l, pixel);
            printf("pixel2");
        }
        for(int l = 0;l<haar.h;l++) {
            //putpixel(ecran, haar.i + haar.w, haar.j - l, pixel);
            printf("pixel3");
        }
       for(int l = 0;l<haar.w;l++) {
            //putpixel(ecran, haar.i - l, haar.j + haar.h, pixel);
            printf("pixel4");
        }*/
       SDL_SetRenderDrawColor(ecran, 0, 255, 0, 255);
       SDL_Rect pos = {haar.i-haar.w, haar.j-haar.h, haar.w, haar.h};
       SDL_RenderDrawRect(ecran, &pos);/*, SDL_MapRGBA(ecran->format, 0, 255, 0,50));*/
    }
}


int main() {
    haarRecord h1; h1.haar = 0; h1.i = h1.j = 110; h1.w = h1.h = 20; h1.value = 0;
    haarRecord h2; h2.haar = 0; h2.i = h2.j = 40; h2.w = h2.h = 40; h2.value = 0;
    haarRecord h3; h3.haar = 0; h3.i = h3.j = 50; h3.w = h3.h = 40; h3.value = 0;

    haarRecord* h = malloc(3*sizeof(haarRecord)); h[0] = h1; h[1] = h2;; h[2] = h3;

    char* imagecopy = copy_filename("./Images/billy.png");
    printf("New file = %s\n",imagecopy);
    //copy_file("./Images/billy.png",imagecopy);
    SDL_Rect positionFond;
    positionFond.x = 0;
    positionFond.y = 0;
    SDL_Surface *img = load_image("./Images/billy.jpg");
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Surface *ecran = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE);
    drawRect(h,img);
    SDL_BlitSurface(img, NULL, ecran, &positionFond);
    SDL_Flip(ecran);
    getchar();
    SDL_Quit();
    return 0;
}

