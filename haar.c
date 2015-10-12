//
//  haar.c
//  
//
//  Created by TheFaceIsALie on 10/10/2015.
//
//

#include "image.c"



struct haarRecord
{
    int haar;
    unsigned long i , j;
    int w,h;
    int value;
};

typedef struct haarRecord haarRecord;


int calcul_area(int x,int y,int x2,int y2,int** mat) {
    int a = mat[x][y];
    //i,j
    int b = mat[x][y2];
    //i,j-1
    int c = mat[x2][y];
    //i-1;j
    int d = mat[x2][y2];
    //i-1;j-1
    //printf("a=%d, b=%d, c=%d, d=%d\n", x, y, x2, y2);
    return (a + d - b - c);
}

int feature1(int x,int y,int x2,int y2,int** mat) {
    int a = calcul_area(x,y,x2,((y - y2)/2),mat);
    int b = calcul_area(((x - x2) / 2),y,x2,y2,mat);
    return (a - b);
}

int haarProcess(int** integralImage, int x, int y, int w, int feature) { //tabulate
        switch (feature) {
            case 1:
                //printf("x=%d, y=%d, w=%d", x, y, w);
                return feature1(x, y, x - w, y - w, integralImage);
                break;
                
            default:
                return 0;
                break;
        }
}
//struct haarRecord*
void processImage(SDL_Surface *image) {
    int** integralImage = matrix_integralImage(image);
    int current_size = 24;
    //int current_size2 = 24;
    int f = 0;
    int value;
    haarRecord *haarOutput;
    haarOutput = malloc((image->h*image->w)*5*(sizeof(struct haarRecord)));
   // display_matrix(image, integralImage);
    while((current_size < image->h) && (current_size < image->w)) {
        //printf("%d\n",current_size);
        for(int i = current_size; i < image->w-1 ; i++) {
            for(int j = current_size; j < image->h-1 ; j++) {
                for(int n = 1; n < 2; n++ ) {
                    //printf("%d,%d,%d\n", i, j, current_size);
                    printf("call");
                    value = haarProcess(integralImage , i, j, current_size, 1);
                    printf("called");
                    //printf("%d\n",value);
                    if(value > 0)
                    {
                        haarOutput[f].value = value;
                        haarOutput[f].haar = n;
                        haarOutput[f].i = i;
                        haarOutput[f].j = j;
                        haarOutput[f].w = current_size;
                        haarOutput[f].h = current_size;
                        //printf("record %d done\n", f);
                        f = f + 1;
                    }
                }
            }
        }
        printf("MULT, %d\n",current_size);
        current_size *= 1.25;
    }
    free(haarOutput);
}
