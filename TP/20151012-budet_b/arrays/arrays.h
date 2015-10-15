//
//  arrays.h
//  
//
//  Created by Benjamin Budet on 13/10/2015.
//
//

/* file: arrays.h */
/* Headers of functions on arrays */

#include <stdio.h>
# ifndef _20151012_ARRAYS_H_
# define _20151012_ARRAYS_H_

/* print_array(begin, end) prints the ints stored between begin and end */
void print_array(int *begin, int *end);

/* binsearch(x, begin, end) returns the pointer to the position of x,
 * the array between begin and end is supposed to be sorted.
 * binsearch returns the expected place of x: the pointer to the first cell
 * containing a value not smaller than x. If all cells contains smaller value,
 * the function returns end. */
int* binsearch(int x, int *begin, int *end);

/* sorted_insert(x, begin, end) insert x at its place between begin and end,
 * the stored values are supposed to sorted in increasing order.
 * The function returns true (!= 0) if x wasn't in the array and false (0)
 * otherwise.
 * We assume that there is enough memory for an insertion. */
int sorted_insert(int x, int *begin, int *end);

/* quick_sort(begin, end) sort the array between begin and end using the quick
 * sort algorithm. */
void quick_sort(int *begin, int *end);

# endif /* _20151012_ARRAYS_H_ */