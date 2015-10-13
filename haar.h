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

int feature1(int x,int y,int x2,int y2,int** mat);
int haarProcess(int** integralImage, int x, int y, int w, int feature);
struct haarRecord processImage(SDL_Surface *image);
int calcul_area(int x,int y,int x2,int y2,int** mat);

