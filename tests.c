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

<<<<<<< HEAD

void drawRect(haarRecord* haarTab, SDL_Surface *ecran) {
=======
void drawRect(haarRecord* haarTab, SDL_Surface *ecran) { //ADD -1 
>>>>>>> ac49190e113e7f7f357e01c8657d81eac600c819
    for(size_t j = 0; haarTab[j].haar != -1;j++) {
        haarRecord haar = haarTab[j];
        printf("i: %zu\n",j);
        SDL_Rect pos = {haar.i-haar.w, haar.j-haar.h, haar.w, 1};
        SDL_FillRect(ecran, &pos, SDL_MapRGB(ecran->format, 0, 255, 0));
        SDL_Rect pos1 = {haar.i-haar.w, haar.j-haar.h, 1, haar.h};
        SDL_FillRect(ecran, &pos1, SDL_MapRGB(ecran->format, 0, 255, 0));
        SDL_Rect pos2 = {haar.i-haar.w, haar.j, haar.w, 1};
        SDL_FillRect(ecran, &pos2, SDL_MapRGB(ecran->format, 0, 255, 0));
        SDL_Rect pos3 = {haar.i, haar.j-haar.h, 1, haar.h};
        SDL_FillRect(ecran, &pos3, SDL_MapRGB(ecran->format, 0, 255, 0));
    }
}


/*int main() {
    haarRecord h1; h1.haar = 0; h1.i = h1.j = 110; h1.w = h1.h = 20; h1.value = 0;
    haarRecord h2; h2.haar = 0; h2.i = h2.j = 40; h2.w = h2.h = 40; h2.value = 0;
    haarRecord h3; h3.haar = -1; h3.i = h3.j = 50; h3.w = h3.h = 40; h3.value = 0;

    haarRecord* h = malloc(3*sizeof(haarRecord)); h[0] = h1; h[1] = h2;; h[2] = h3;

    char* imagecopy = copy_filename("./Images/billy.jpg");
    printf("New file = %s\n",imagecopy);
    copy_file("./Images/billy.png",imagecopy);
    SDL_Rect positionFond;
    positionFond.x = 0;
    positionFond.y = 0;
    SDL_Surface *img = load_image("./Images/billyCopy.jpg");
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Surface *ecran = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE);
    drawRect(h,img);
    SDL_BlitSurface(img, NULL, ecran, &positionFond);
    SDL_Flip(ecran);
    getchar();
    SDL_Quit();
    return 0;
}*/

