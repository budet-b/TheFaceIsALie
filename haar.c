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
                //printf("feature1\n");
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
    int f = 0;
    int value;
    haarRecord *haarOutput;
    haarOutput = malloc((image->h*image->w)*5*(sizeof(struct haarRecord)));
   // display_matrix(image, integralImage);
    
    for(int i = current_size; i < image->w ; i++) {
        for(int j = current_size; j < image->h ; j++) {
            for(int n = 1; n < 2; n++ ) {
            
                //printf("%d,%d,%d\n", i, j, n);
                value = haarProcess(integralImage , i, j, current_size, 1);
                //printf("%d\n",value);
                if(value > 0)
                {
                    haarOutput[f].value = value;
                    haarOutput[f].haar = n;
                    haarOutput[f].i = i;
                    haarOutput[f].j = j;
                    haarOutput[f].w = current_size;
                    haarOutput[f].h = current_size;
                  //  printf("record %d done\n", f);
                    f = f + 1;
                }
            }
        }
       /* int ntm = 0;
        while (*haarOutput[ntm]!= NULL) {
            free(*haarOutput[ntm]);
            //free(haarOutput);
            ntm++;
        */
       // free(haarOutput);
    }
}