//
//  arrays.c
//  
//
//  Created by Benjamin Budet on 13/10/2015.
//
//

#include "arrays.h"
#include <stdio.h>
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
    if(end - begin < 1)
        return begin;
    int *mid = begin + ((end-begin)/2);
    if(*mid < x){
        return binsearch(x,(mid+1),end);
    }
    else{
        return binsearch(x,begin,mid);
    }
}

/* sorted_insert(x, begin, end) insert x at its place between begin and end,
 * the stored values are supposed to sorted in increasing order.
 * The function returns true (!= 0) if x wasn't in the array and false (0)
 * otherwise.
 * We assume that there is enough memory for an insertion. */

int sorted_insert(int x, int *begin, int *end){
    int *pos = binsearch(x,begin,end);
    if (pos < begin && (*pos == x)){
        return 0;
    }
    if (pos < end){
        memmove(pos+1, pos, (end - pos)*sizeof(int));
    }
    *pos = x;
    return 1;
}

int *max (int *x, int *y){
    if (x > y)
        return x;
    else
        return y;
}

int *min (int *x, int *y ){
    if (x < y)
        return x;
    else
        return y;
}

int *choose_pivot(int *begin, int *end){
   return ((end - begin) / 2 + begin);
}

int swap(int *a, int *b) {
    int c = *a;
    *a = *b;
    *b = c;
    return 0;
}

int* partition(int *begin, int *end, int *pivot){
    int pval = *pivot;
    swap(pivot, end-1);
    pivot = begin;
   for (;begin <(end - 1);begin++){
        if(*begin < pval){
            swap(pivot, begin);
            pivot++;
        }
    }
    swap(pivot, end - 1);
    return pivot;
}

/* quick_sort(begin, end) sort the array between begin and end using the quick
 * sort algorithm. */

void quick_sort(int *begin, int *end){
    if (begin < end) {
        int *pivot = begin + (end - begin) / 2;
        pivot = partition(begin, end, pivot);
        quick_sort(begin, pivot);
        quick_sort(pivot + 1, end);
    }
}
