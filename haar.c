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
    int a = calcul_area(x,y,x2,((y - y2-1)/2),mat);
    int b = calcul_area(((x - x2-1) / 2),y,x2,y2,mat);
    return (a - b);
}

/*int feature2(int x, int y, int x2, int y2, int** mat) {
    int a = calcul_area(x, y,  
    int b = calcul_area
    return (a - b);
}*/

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
    haarRecord *haarOutputTab;
    haarRecord *haarOutput = NULL;
    
    haarOutputTab = malloc((image->h*image->w)*20*(sizeof(struct haarRecord)));
   // display_matrix(image, integralImage);
    while((current_size < image->h) && (current_size < image->w)) {
        //printf("%d\n",current_size);
        for(int i = current_size; i < image->w ; i++) {
            for(int j = current_size; j < image->h ; j++) {
                for(int n = 1; n < 2; n++ ) {
                    //printf("%d,%d,%d\n", i, j, current_size);
                    value = haarProcess(integralImage , i, j, current_size, n);
                    //printf("%d\n",value);
                    if(value > 0)
                    {
                        haarOutput = malloc(sizeof(struct haarRecord));
                        haarOutput->value = value;
                        haarOutput->haar = n;
                        haarOutput->i = i;
                        haarOutput->j = j;
                        haarOutput->w = current_size;
                        haarOutput->h = current_size;
                        printf("Record %d done\n", f);
                        haarOutputTab[f] = *haarOutput;
                        free(haarOutput);
                        f = f + 1;
                    }
                }
            }
        }
        printf("MULT, %d\n",f);
        current_size *= 1.25;
    }
    
    for(int derp = 0; derp <f; derp++)
        printf("%d\n", haarOutputTab[derp].value);
    free(haarOutputTab);
}
