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
    int weight;
};

typedef struct haarRecord haarRecord;


int calcul_area(int x,int y,int x2,int y2,int** mat) {
    //printf("a=%d, b=%d, c=%d, d=%d\n", x, y, x2, y2);
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
//Rectangle : Noir à gauche, blanc droite
int feature1(int x,int y, int h, int w, int** mat) {
    int a = calcul_area(x, y, x - h, y - (w / 2), mat);
    int b = calcul_area(x, y - (w / 2), x - h, y - w, mat);
    return (a - b);
}
//Rectangle : Noir en bas, blanc en haut
int feature2(int x, int y, int h, int w, int** mat) {
    int a = calcul_area(x, y, x-(h/2), y-w, mat);
    int b = calcul_area(x-(h/2), y, x-h, y-w, mat);
    return (a - b);
}
//Rectangle : Noir au mileu, blanc des deux autres cotés VERTICAL
int feature3(int x, int y, int h, int w, int** mat) {
    int a = calcul_area(x, y, x-h, y-(w/3),mat);
    int b = calcul_area(x, y-((2*w)/3), x-h, y-w, mat);
    int c = calcul_area(x, y-(w/3), x-h, y-((2*w)/3), mat);
    return (c - a - b);
}
//Rectangle : Noir au mileu, blanc des deux autres cotés HORIZONTAL
int feature4(int x, int y, int h, int w, int** mat) {
    int a = calcul_area(x, y, x-(h/3), y-w,mat);
    int b = calcul_area(x-(h/3), y, x-((2*h)/3), y-w, mat);
    int c = calcul_area(x-((2*h)/3), y, x-h, y-w, mat);
    return (b - a -c);
}

int feature5(int x, int y, int h, int w, int** mat) {
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
/*
void processImage(SDL_Surface *image) {
    int** integralImage = matrix_integralImage(image);
    int current_size = 1;
    int current_size2 = 1;
    int current_sizeT = 1;
    int current_size2T = 1;
    int derp = 0;
    int f = 0;
    int value;
    haarRecord *haarOutputTab;
    haarRecord *haarOutput = NULL;
    
    haarOutputTab = malloc((image->h*image->w)*500*(sizeof(struct haarRecord)));
   // display_matrix(image, integralImage);
    while((current_size < 24) && (current_size2 < 24)) {
        printf("%d,%d\n",current_size,current_size2);
        for(int i = current_size; i < 24; i++) {
            for(int j = current_size2; j < 24; j++) {
                for(int n = 1; n<=5; n++ ) {
                    //printf("%d,%d,%d\n", i, j, current_size);
                    value = haarProcess(integralImage , i, j, current_size, current_size2, n);
                    derp = derp + 1;
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
        printf("CHANGE SIZE, %d\n",derp);
        if(current_size == 23) {
            ++current_size2;
            current_size = current_sizeT + 1;
            current_sizeT++;
        }
        else {
            ++current_size;
        }
        if(current_size == 23 && current_size2 == 23) {
            current_size = current_sizeT;
            current_size2 = current_size2T;
            ++current_sizeT;
            ++current_size2T;
        }
        
    }
    
    for(int derp = 0; derp <f; derp++)
        printf("%d\n", haarOutputTab[derp].value);
    free(haarOutputTab);
    
}
*/

haarRecord* processImage(SDL_Surface *image, int* NbFeatures) {
    const int imageW = image->w;
    const int imageH = image->h;
	const int frameSize = 24;
	const int features = 5;
	//All five feature types:
    const int feature[5][2] = {{1,2}, {2,1}, {3,1}, {1,3}, {2,2}};
	int** integralImage = matrix_integralImage(image);

	int f = 0;
	int value;
	haarRecord *haarOutputTab;
	haarRecord *haarOutput = NULL; 
	haarOutputTab = malloc((image->h*image->w)*500*(sizeof(struct haarRecord)));

	int count = 0;
	//Each feature:
	for (int i = 0; i < features; i++) {
    	int sizeX = feature[i][0];
    	int sizeY = feature[i][1];
    	
        //Each detection zone within framesize posible
        for (int width = sizeX - 1; width < frameSize; width+=sizeX){
        	for (int height = sizeY - 1; height < frameSize; height+=sizeY){
                //printf("\tfeature %d, size: %dx%d\n", i, width, height);
            	//Each position fiting in framesize with current detection zone
            	for (int x = width; x < imageW; x++) {
                	for (int y = height; y < imageH; y++) {
		            	//printf("pos: %d,%d\n", x, y);
                        count++;
						value = haarProcess(integralImage , x, y, width, height, i+1);
						//printf("%d\n",value);
						if(value > 0) {
							haarOutput = malloc(sizeof(struct haarRecord));
							haarOutput->value = value;
							haarOutput->haar = i + 1;
							haarOutput->i = x;
							haarOutput->j = y;
							haarOutput->w = width;
							haarOutput->h = height;
							//printf("Record %d done\n", f);
							haarOutputTab[f] = *haarOutput;
							free(haarOutput);
							f = f + 1;
						}
                	}
            	}
            }
    	}
	}
    printf("%d",count);
    *NbFeatures = f;
    return(haarOutputTab);
}
