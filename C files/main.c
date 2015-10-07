#include <stdlib.h>
#include <stdio.h>
#include "image.c"
#include "image.h"
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>



int main(int argc, char* argv[]) {
	printf("THE FACE IS A LIE\n");
	if (argc < 2)
    		errx(2, "Usage:\n%s <path>", argv[0]);
	SDL_Surface* image = load_image(argv[1]);
	ToGray(image);
    Binarize(image);
    int** integralMatrix = matrix_integralImage(image);
	SDL_SaveBMP(image, "./ToGray.bmp");
	return 0;
}
