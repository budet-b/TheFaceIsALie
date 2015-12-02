# include <stdio.h>
# include <stdlib.h>


void fill_matrix(int M[], size_t m, size_t n) {
    for(size_t i = 0; i < m; i++)
        for (size_t j = 0; j < n; j++) 
            M[i*n+j] = j * m + i;
}


void print_matrix(int M[], size_t m, size_t n) {
    fill_matrix(M,m,n);
    for (size_t i = 0; i < m; i++) {
        size_t line_offset = i * n;
        for (size_t j = 0; j < n; j++) {
            printf("| %3d ", M[line_offset + j]);
        }
        printf("|\n");
    }
}


int main(int argc, char *argv[]) {
    size_t m = 3;
    size_t n = 5;
    if (argc == 3) {
        m = strtoul(argv[1], NULL, 10);
        n = strtoul(argv[2], NULL, 10);
    }
    int *M = malloc(sizeof(int)*n);
    print_matrix(M,m,n);
    return 0;
}
