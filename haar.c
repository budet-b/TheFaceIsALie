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
//Rectangle : Noir à gauche, blanc droite
int feature1(int x,int y, int w, int h, int** mat) {
    int a = calcul_area(x, y, x - h, y - (w / 2), mat);
    int b = calcul_area(x, y - (w / 2), x - h, y - w, mat);
    return (a - b);
}
//Rectangle : Noir en bas, blanc en haut
int feature2(int x, int y, int w, int h, int** mat) {
    int a = calcul_area(x, y, x-(h/2), y-w, mat);
    int b = calcul_area(x-(h/2), y, x-h, y-w, mat);
    return (a - b);
}
//Rectangle : Noir au mileu, blanc des deux autres cotés VERTICAL
int feature3(int x, int y, int w, int h, int** mat) {
    int a = calcul_area(x, y, x-h, y-(w/3),mat);
    int b = calcul_area(x, y-((2*w)/3), x-h, y-w, mat);
    int c = calcul_area(x, y-(w/3), x-h, y-((2*w)/3), mat);
    return (c - a - b);
}
//Rectangle : Noir au mileu, blanc des deux autres cotés HORIZONTAL
int feature4(int x, int y, int w, int h, int** mat) {
    int a = calcul_area(x, y, x-(h/3), y-w,mat);
    int b = calcul_area(x-(h/3), y, x-((2*h)/3), y-w, mat);
    int c = calcul_area(x-((2*h)/3), y, x-h, y-w, mat);
    return (b - a -c);
}

int feature5(int x, int y, int w, int h, int** mat) {
    int a = calcul_area(x, y, (x-h)/2, y - (w/2),mat);
    int b = calcul_area(x, y - (w/2), x - (h/2), y - w, mat);
    int c = calcul_area(x - (h/2), y, x - h, y - (w/2), mat);
    int d = calcul_area(x - (h/2), y - (w/2), x - h, y - w ,mat);
    return (b + c - a - d);
}

int haarProcess(int** integralImage, int x, int y, int w, int h, int feature) { //tabulate
        switch (feature) {
            case 1:
                //printf("x=%d, y=%d, w=%d", x, y, w);
                return feature1(x, y, w, h, integralImage);
                break;
            
            case 2:
                return feature2(x, y, w, h, integralImage);
                break;
                
            case 3:
                return feature3(x, y, w, h, integralImage);
                break;
                
            case 4:
                return feature4(x, y, w, h, integralImage);
                break;
                
            case 5:
                return feature5(x , y , w, h, integralImage);
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
    int current_size2 = 24;
    int f = 0;
    int value;
    haarRecord *haarOutputTab;
    haarRecord *haarOutput = NULL;
    
    haarOutputTab = malloc((image->h*image->w)*20*(sizeof(struct haarRecord)));
   // display_matrix(image, integralImage);
    while((current_size < image->h) && (current_size < image->w)) {
        //printf("%d\n",current_size);
        for(int i = current_size; i < image->w ; i++) {
            for(int j = current_size2; j < image->h ; j++) {
                for(int n = 1; n < 6; n++ ) {
                    //printf("%d,%d,%d\n", i, j, current_size);
                    value = haarProcess(integralImage , i, j, current_size, current_size2, n);
                    //printf("%d\n",value);
                    if(value > 0)
                    {
                        haarOutput = malloc(sizeof(struct haarRecord));
                        haarOutput->value = value;
                        haarOutput->haar = n;
                        haarOutput->i = i;
                        haarOutput->j = j;
                        haarOutput->w = current_size;
                        haarOutput->h = current_size2;
                        //printf("Record %d done\n", f);
                        haarOutputTab[f] = *haarOutput;
                        free(haarOutput);
                        f = f + 1;
                    }
                }
            }
        }
       // printf("MULT, %d\n",f);
        current_size *= 1.25;
        current_size2 *= 1.25;
    }
    
    /*for(int derp = 0; derp <f; derp++)
        printf("%d\n", haarOutputTab[derp].value);*/
    free(haarOutputTab);
}
