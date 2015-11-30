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
#include "adabase.h"
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
    strcat(filepath,"DB.txt");
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
    int face = 0, notface = 0, i = 0;
    while(i<size) {
        int r = rand() % 2;
        if(r==1 && pathFace[face] != '\0' && face<(size/2)) {
            visage[i] = 1;
            finalpath[i] = pathFace[face];
            face++;
            i++;
        }
        else if (r==0 && pathNotFace[notface] != '\0' && notface<(size/2)) {
            visage[i] = -1;
            finalpath[i] = pathNotFace[notface];
            notface++;
            i++;
        }
    }
}

int main(int argc, char* argv[]) {
    
    FILE *database = NULL;
    FILE *classifier = NULL;
    
    if(argc < 1)
        errx(2, "Insuffisant argument");

    if(strcmp(argv[1], "train") == 0) {
        int visage[MAXLINES];
        char* pathface[MAXLINES];
        files(visage,pathface,"./Images/BigDB/Face/");
        char* pathnotface[MAXLINES];
        files(visage,pathnotface,"./Images/BigDB/NotFace/");
        char* finalpath[MAXLINES];
        randFace(visage,pathface,pathnotface,200,finalpath);
        printf("Starting Training\n");
        strongClassifier* result;
        result = adaboost(finalpath, visage, 100, 100, 3);
        printf("Training Finished, Writing Classifier\n");
        writeClassifier(result,classifier);
        printf("Classifier writed\n");
        return 0;
    }

    if(argc < 2)
        errx(2, "Insuffisant argument");

    if((database = (fopen("database", "a+"))) == NULL) {
        fprintf(stderr,"Impossible to open file in lecture");
        fclose(database);
    }

    if(strcmp(argv[1], "add") == 0) {
        database = fopen("database", "a+");
        add(argc, argv, database);
        fclose(database);
    }

    if(strcmp(argv[1], "read") == 0) {
        database = fopen("database", "r");
        read(database);
        fclose(database);
    }

    if(strcmp(argv[1], "search") == 0) {
        database = fopen("database","r");
        read(database);
        fclose(database);
    }

    if(strcmp(argv[1], "identify") == 0) {
        /*int checksum = process(argv[2]);
        if(checksum == 1) {
            //Face Detected + Visage identified 
        }
        else if(checksum == -1) {
            //Face Detected + No Visage identified
        }
        else {
            // No Face
        }*/   
    }

    return 0;
}
