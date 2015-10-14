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
            case 0:
                //printf("x=%d, y=%d, w=%d", x, y, w);
                return feature1(x, y, w, h, integralImage);
                break;
            
            case 1:
                return feature2(x, y, w, h, integralImage);
                break;
                
            case 2:
                return feature3(x, y, w, h, integralImage);
                break;
                
            case 3:
                return feature4(x, y, w, h, integralImage);
                break;
                
            case 4:
                return feature5(x , y , w, h, integralImage);
                break;

            default:
                return 0;
                break;
        }
}

/*QUICK SORT
CALL WITH quick_sort( *haarOutputTab[n]) with 0 <= n <= 4
void quick_sort (haarRecord *a, int n) {
    int i,j;
    haarRecord p, t;
    if (n < 2)
        return;
    p.value = (a[n / 2]).value;
    for (i = 0, j = n - 1;; i++, j--) {
        while (a[i].value < p.value)
            i++;
        while (p.value < a[j].value)
            j--;
        if (i >= j)
            break;
        t = a[i];
        a[i] = a[j];
        a[j] = t;
    }
    quick_sort(a, i);
    quick_sort(&(a[i]), n - i);
}
*/
void haarRecording(haarRecord *haarOutputTab[], int value, int i, int j, int current_size, int current_size2, int n, int f) {
    haarRecord *haarOutput = NULL;
    haarOutput = malloc(sizeof(struct haarRecord));
    haarOutput->value = value;
    haarOutput->haar = n;
    haarOutput->i = i;
    haarOutput->j = j;
    haarOutput->w = current_size;
    haarOutput->h = current_size2;
    //printf("Record %d done\n", f);
    haarOutputTab[n][f] = *haarOutput;
    free(haarOutput);
}
//struct haarRecord*
void processImage(SDL_Surface *image) {
    int** integralImage = matrix_integralImage(image);
    int current_size = 24;
    int current_size2 = 24;
    int f1 = 0;
    int f2 = 0;
    int f3 = 0;
    int f4 = 0;
    int f5 = 0;
    int value;
    haarRecord **haarOutputTab = malloc((image->h*image->w)*(sizeof(struct haarRecord)));
    printf("derp\n");
    for (int i = 0; i < 5; i++) {
        haarOutputTab[i] = malloc((image->h*image->w)*(sizeof(struct haarRecord)));
    }
    printf("derp2\n");
   // display_matrix(image, integralImage);
    while((current_size < image->h) && (current_size2 < image->w)) {
        //printf("%d\n",current_size);
        for(int i = current_size; i < image->w ; i++) {
            for(int j = current_size2; j < image->h ; j++) {
                for(int n = 0; n < 5; n++ ) {
                    //printf("%d,%d,%d\n", i, j, current_size);
                    value = haarProcess(integralImage , i, j, current_size, current_size2, n);
                    //printf("%d\n",value);
                    if(value > 0) {
                        switch (n) {
                            case 0:
                                haarRecording(haarOutputTab, value, i, j, current_size, current_size2, n, f1);
                                f1 = f1 + 1;
                                break;
                            case 1:
                                haarRecording(haarOutputTab, value, i, j, current_size, current_size2, n, f2);
                                f2 = f2 + 1;
                                break;
                            case 2:
                                haarRecording(haarOutputTab, value, i, j, current_size, current_size2, n, f3);
                                f3 = f3 + 1;
                                break;
                            case 3:
                                haarRecording(haarOutputTab, value, i, j, current_size, current_size2, n, f4);
                                f4 = f4 + 1;
                                break;
                            case 4:
                                haarRecording(haarOutputTab, value, i, j, current_size, current_size2, n, f5);
                                f5 = f5 + 1;
                                break;
                            default:
                                break;
                        }
                    }
                }
            }
        }
        printf("MULT, %d\n, %d\n, %d\n, %d\n, %d\n",f1, f2, f3, f4, f5);
        current_size *= 1.25;
        current_size2 *= 1.25;
    }
    /*for(int n = 0; n < 5; n++) {
        int x = sizeof haarOutputTab[n] / sizeof haarOutputTab[n][0];
        quick_sort(haarOutputTab[n], x);
    }
    for(int i = 0; i < f1; i++) {
        printf("| %d |\n",haarOutputTab[0][i].value);
    }*/
    free(haarOutputTab);
}
