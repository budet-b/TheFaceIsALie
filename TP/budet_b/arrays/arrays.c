//
//  arrays.c
//  
//
//  Created by Benjamin Budet on 13/10/2015.
//
//
#include "arrays.h"
#include <string.h>
/* print_array(begin, end) prints the ints stored between begin and end */
void print_array(int *begin, int *end) {
    for (; begin < end ; begin++) {
        printf("%3d |",*begin);
    }
}
/* binsearch(x, begin, end) returns the pointer to the position of x,
 * the array between begin and end is supposed to be sorted.
 * binsearch returns the expected place of x: the pointer to the first cell
 * containing a value not smaller than x. If all cells contains smaller value,
 * the function returns end. */
int* binsearch(int x, int *begin, int *end) {
    while (end - begin > 0) {
        int *mid = begin + (end - begin) / 2;
        if (*mid == x)
            return mid;
        if (*mid < x)
            begin = mid + 1;
        else
            end = mid;
    }
    return NULL;
}
/* sorted_insert(x, begin, end) insert x at its place between begin and end,
 * the stored values are supposed to sorted in increasing order.
 * The function returns true (!= 0) if x wasn't in the array and false (0)
 * otherwise.
 * We assume that there is enough memory for an insertion. */
int sorted_insert(int x, int *begin, int *end){
    int pos = binsearc(x,begin,end);
    if (pos<begin && (pos == x)) {
        return 1;
    }
    if (pos<begin) {
        memmove(end,pos,1);
    }
    pos = x;
    return 1;
}


/*
int* max (int x,int y){
    if (x>y)
        return *x;
    else
        return *y;
}

int* min (int x,int y ){
    if (x<y)
        return *x;
    else
        return *y;
}
*/
int* choose_pivot(int *begin, int *end, int *pivot){
    int *mid = (begin +(end - begin)/2);
    return (max(min(begin,end),min(max(begin,end),mid)));
}
void swap(int *a, int *b) {
    int c = *a;
    *a = *b;
    *b = c;
}
int* partition(int *begin, int *end, int *pivot){
    int pval = *pivot;
    swap(pivot,end-1);
    pivot = begin;
  /*  for (;begin <(end-1);begin++){
        if(*begin<pval){
            swap(pivo,begin);
            pivot++;
        }
    }*/
    for (int *k = begin; k < end - 1; ++k) {
        if (*k <= pval) {
            swap(k, pivot);
            pivot++;
        }
    }
    swap(pivot, end - 1);
    return pivot;
}

/* quick_sort(begin, end) sort the array between begin and end using the quick
 * sort algorithm. */
void quick_sort(int *begin, int *end){
    /*   int mid = (begin+(end-begin)/2);
     if (begin < end) {
     int key = *begin;
     int low = end;
     int high = begin;
     while ((low < high && *end) >= key) {
     high--;
     }
     *begin = *end;
     while ((low < high && *end)<= key) {
     low++;
     }
     *end = *begin;
     }
     *begin =key;
     quick_sort(begin,end-1);
     quick_sort(low+1,end); */
    
    if (begin < end) {
        int *pivot = begin + (end - begin) / 2;
        pivot = partition(begin, end, pivot);
        quick_sort(begin, pivot);
        quick_sort(pivot + 1, end);
    }
}