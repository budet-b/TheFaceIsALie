#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <unistd.h>
#include <err.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <float.h>
#include <limits.h>
#include <ctype.h>

SDL_Surface* load_image(char *path);


    static inline
Uint8* pixelref(SDL_Surface *image, unsigned x, unsigned y); 

Uint32 getpixel(SDL_Surface *image, unsigned x, unsigned y);

void ToGray(SDL_Surface *surface);

void putpixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel);

void Binarize(SDL_Surface *surface);



