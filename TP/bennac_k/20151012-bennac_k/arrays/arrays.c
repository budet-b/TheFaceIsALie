#include "arrays.h"
#include <string.h>
#include <stdio.h>

void print_array(int *begin, int *end){
    printf("| ");
    for(;begin<end;begin++)
        printf("%d | ",*begin);
}


int* binsearch(int x, int *begin, int *end){
    if(end - begin < 1)
		return begin;
	int *mid = begin + ((end-begin)/2);
	if(*mid < x)
		return binsearch(x,(mid+1),end);
	else
		return binsearch(x,begin,mid);
}


int sorted_insert(int x, int *begin, int *end){
    int *pos = binsearch(x,begin,end);
    if ((pos < end) && (*pos == x) )
        return 0;
    if (pos < end)
        end = memmove(pos+1,pos,(end-pos)*sizeof(int));
    *pos = x;
    return 1;
}


int *min(int *a, int *b){
    if (a>b)
        return b;
    return a;
}


int *max(int *a, int *b){
    if (a>b)
        return a;
    return b;
}

int *choose_pivot(int *begin, int *end) {
   return (end - begin) / 2 + begin;
}


int swap(int* a,int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
    return 1;
}


int* partition(int *begin, int *end, int *pivot) {
    int pval = *pivot;
    swap(pivot,end-1);
    pivot = begin;
    for(;begin<end-1;begin++) {
        if(*begin < pval) {
            swap(pivot, begin);
            pivot++;
        }
    }
    swap(pivot,end-1);
    return pivot;
}


void quick_sort(int *begin, int *end) {
    int *pivot = NULL;
    if (end-begin > 1) {
        pivot = choose_pivot(begin,end);
        pivot = partition(begin,end,pivot);
        quick_sort(begin,pivot);
        quick_sort(pivot+1, end);
    }
} 
