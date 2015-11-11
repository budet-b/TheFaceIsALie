#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <err.h>

#include "image.h"
#include "haar.h"
#include "adaboost.h"

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

int main(int argc, char* argv[]) {
    char* result[] = { "./Images/DB/Resize/test00.png", "./Images/DB/Resize/test01.png", "./Images/DB/Resize/test02.png","./Images/DB/Resize/test03.png", "./Images/DB/Resize/test04.png" };
    int visage[5] = { 1, 1, 1, -1, -1};
	printf("THE FACE IS A LIE\n");
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
    yolo = adaboost(result, visage, 3, 2, 10);
    
    //int len;
    //haarRecord* haarOutput;
    //haarOutput = processImage(image, &len);
    //display_haar(haarOutput, len);
    //SDL_SaveBMP(image, "./ToGray.bmp");
    return 0;
}
