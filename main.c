#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <err.h>
#include <string.h>
#include <time.h>

#include "image.h"
#include "haar.h"
#include "adaboost.h"
#define MAXLINES 2500

char lines[MAXLINES][MAXLINES];

void wait_for_keypressed(void) {
  SDL_Event             event;
  // Infinite loop, waiting for event
  for (;;) {
    // Take an event
    SDL_PollEvent( &event );
    // Switch on event type
    switch (event.type) {
    // Someone pressed a key -> leave the function
    case SDL_KEYDOWN: return;
    default: break;
    }
  // Loop until we got the expected event
  }
}

SDL_Surface* display_image(SDL_Surface *img) {
  SDL_Surface          *screen;
  // Set the window to the same size as the image
  screen = SDL_SetVideoMode(img->w, img->h, 0, SDL_SWSURFACE|SDL_ANYFORMAT);
  if ( screen == NULL ) {
    // error management
    errx(1, "Couldn't set %dx%d video mode: %s\n",
         img->w, img->h, SDL_GetError());
  }

  /* Blit onto the screen surface */
  if(SDL_BlitSurface(img, NULL, screen, NULL) < 0)
    warnx("BlitSurface error: %s\n", SDL_GetError());

  // Update the screen
  SDL_UpdateRect(screen, 0, 0, img->w, img->h);

  // wait for a key
  wait_for_keypressed();

  // return the screen for further uses
  return screen;
}


void display_haar(haarRecord* tab, int nbFeature) {
    for(int i = 0; i < nbFeature; i++) {
        printf("| %d |\n", tab[i].value);
    }
}


int* faces(int valid, int invalid) {
    static int visage[MAXLINES];
    for(int i = 0; i < valid; i++)
        visage[i] = 1;
    for(int i = valid; i < invalid; i++)
        visage[i] = -1;
    return visage;
}


void files(int *visage, char* path[], char *paths) {

    int i = 0;
    char filepath[MAXLINES];
    strcpy(filepath,paths);
    strcat(filepath,"DBList.txt");
    FILE *fp = fopen(filepath, "r");

    if (fp == 0) {
        printf("%s\n",filepath);
        fprintf(stderr, "failed to open DBtexte\n");
        exit(1);
    }

    while (i < MAXLINES && fgets(lines[i], sizeof(lines[0]), fp)) {
        lines[i][strlen(lines[i])-1] = '\0';
        i = i + 1;
    }

    i = 0;
    while(strcmp(lines[i],"\0"))
        i++;

    char *folder = paths;

    for (size_t j = 0; j<(size_t)i; j++) {
        char *temp = lines[j];
        char *temp2 = (char *) malloc(1 + strlen(folder)+ strlen(temp));
        strcat(temp2,folder);
        strcat(temp2,temp);
        path[j] = temp2;
    }
    fclose(fp);
    visage = faces(i-1,i);
}

void randFace(int visage[], char* pathFace[], char* pathNotFace[],int size,char* finalpath[]) {
    srand(time(NULL));
    for(int i = 0;i<size;i++) {
        int r = rand() % 2;
        if(r==1 && pathFace[i] != '\0') {
            visage[i] = 1;
            finalpath[i] = pathFace[i];
        }
        else if (r==0 && pathNotFace[i] != '\0') {
            visage[i] = -1;
            finalpath[i] = pathNotFace[i];
        }
    }
}

int main(int argc, char* argv[]) {
    int visage[MAXLINES];
    char* pathface[MAXLINES];
    files(visage,pathface,"./Images/Face/");
    char* pathnotface[MAXLINES];
    files(visage,pathnotface,"./Images/NotFace/");
    /*for(size_t i = 0;i<125;i++) {
        printf("%s\n",pathface[i]);
        printf("%s\n",pathnotface[i]);
    }*/
    char* finalpath[MAXLINES];
    randFace(visage,pathface,pathnotface,125,finalpath);
    for(size_t i = 0;i<125;i++) {
        printf("%s ",finalpath[i]);
        printf("%d\n",visage[i]);
    }

    printf("\nTHE FACE IS A LIE\n");
	/*if (argc < 2)
    		errx(2, "Usage:\n%s <path>", argv[0]);
	SDL_Surface* image = load_image(argv[1]);
	display_image(image);
	ToGray(image);
	display_image(image);
    Binarize(image);
	display_image(image); */
    printf("ONSTART\n");
    strongClassifier* yolo;
    printf("yolo adaboost");
    //yolo = adaboost(path, visage, 124, 1, 25);
    
    //int len;
    //haarRecord* haarOutput;
    //haarOutput = processImage(image, &len);
    //display_haar(haarOutput, len);
    //SDL_SaveBMP(image, "./ToGray.bmp");
    return 0;
}
