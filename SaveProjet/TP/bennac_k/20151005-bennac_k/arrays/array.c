# define _XOPEN_SOURCE 500
 
# include <assert.h>
# include <err.h>
# include <stdio.h>
# include <stdlib.h>
 
/* Provided code for testing and inspiration */
 
static inline
void swap(int *a, int *b) {
  int c = *a;
  *a = *b;
  *b = c;
}
 
static
void fill_array(int arr[], size_t len) {
  for (size_t i = 0; i < len; i++)
    arr[i] = random() % 1000;
}
 
static
void print_array(int arr[], size_t len) {
  for (size_t i = 0; i < len; i++) {
    printf("| %3d ", arr[i]);
  }
  printf("|\n");
}
 
/**********************************/
/* Code to be completed		  */
/* See subject for specifications */
/**********************************/
 
int* min_pos(int arr[], size_t len) {
  /* FIXED */
    int pos = 0;
    for(unsigned long i = 0; i<len; i++){
        if(arr[i] < arr[pos])
            pos = i;
    }        
    return &(arr[pos]);
}
 
void select_sort(int arr[], size_t len) {
  /* FIXED */
    for(unsigned long i = 0; i < len;i++) { 
        for(unsigned long j = i+1;j<len;j++)
        {
            if(arr[i]>arr[j])
                swap(&(arr[i]),&(arr[j]));
        }
    }
}
 
int is_sorted(int arr[], size_t len) {
  /* FIXED */
    for(unsigned long i = 0; i < len-1;i++) {
        if(&(arr[i]) > &(arr[i+1]))
            return 0;
    }
    return 1;
}
 
/* entry point */
 
int main(int argc, char *argv[]) {
  // Need a length
  if (argc < 2)
    errx(1, "missing array len");
  size_t len = strtoul(argv[1], NULL, 10);
  // Create and fill array
  int *arr = malloc(len * sizeof (int));
  fill_array(arr, len);
  // Print the array
  print_array(arr, len);
 
  // Sort array using your code
  select_sort(arr, len);
 
  // Print it again
  print_array(arr, len);
 
  // Assertion: verify that array is sorted
  assert(is_sorted(arr, len));
  return 0;
}
