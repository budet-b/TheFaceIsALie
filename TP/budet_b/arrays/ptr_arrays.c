//
//  arrays.c
//  
//
//  Created by Benjamin Budet on 13/10/2015.
//
//


# define _XOPEN_SOURCE 600

# include <assert.h>
# include <stdio.h>
# include <stdlib.h>

//# include "arrays.h"

static inline
int is_sorted(int *begin, int *end) {
    for (; begin < end - 1 && *begin <= *(begin + 1); begin++) {}
    return begin == end - 1;
}

static
void test_sorted_insert(int *arr, size_t len) {
    printf("Sorted fill:\n");
    int *begin = arr, *end = arr;
    while (end - begin < (ssize_t)len) {
        int x = random() % 1000;
        printf("\tInserting %3d: ", x);
        if (sorted_insert(x, begin, end)) {
            printf("OK\n");
            end++;
        } else {
            printf("already inside\n");
        }
    }
    print_array(begin, end);
    printf("Array is sorted: ");
    if (is_sorted(begin, end))
        printf("OK\n");
    else {
        printf("KO\n");
        assert(0);
    }
}

static inline
void random_fill(int *begin, int *end) {
    for (int *cur = begin; cur < end; cur++)
        *cur = random() % 1000;
}

static inline
void sorted_fill(int *begin, int *end) {
    for (int i = 0; begin < end; begin++, i++)
        *begin = i;
}

static inline
void rev_sorted_fill(int *begin, int *end) {
    for (int i = end - begin; begin < end; begin++, i--)
        *begin = i;
}

static
void subtest_qsort(int *begin, int *end) {
    printf("\tbefore sort:\n\t");
    print_array(begin, end);
    quick_sort(begin, end);
    printf("\tafter sort:\n\t");
    print_array(begin, end);
    
    printf("Array is sorted: ");
    if (is_sorted(begin, end))
        printf("OK\n");
    else {
        printf("KO\n");
        assert(0);
    }
}

static
void test_qsort(int *arr, size_t len) {
    int *begin = arr, *end = arr + len;
    
    printf("Sorting sorted arrays:\n");
    sorted_fill(begin, end);
    subtest_qsort(begin, end);
    
    printf("Sorting reverse sorted arrays:\n");
    rev_sorted_fill(begin, end);
    subtest_qsort(begin, end);
    
    printf("Sorting random arrays:\n");
    random_fill(begin, end);
    subtest_qsort(begin, end);
}

int main(int argc, char *argv[]) {
    size_t len = 8;
    if (argc > 1)
        len = strtoul(argv[1], NULL, 10);
    int *arr = malloc(len * sizeof (int));
    printf("*** Testing insertion and binary search ***\n");
    test_sorted_insert(arr, len);
    
    printf("\n*** Testing quick sort***\n");
    test_qsort(arr, len);
    
    free(arr);
    return 0;
}
