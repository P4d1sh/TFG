#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "matrix.h"

int main(int argc, char const *argv[])
{
    /* Intializes random number generator */
    time_t t;
    srand((unsigned)time(&t));
    srand(0);

    const unsigned scale = 1;
    const unsigned rows_a = 2 * scale;
    const unsigned cols_a = 2 * scale;
    const unsigned rows_b = 2 * scale;
    const unsigned cols_b = 2 * scale;

    // Reserve memory
    double *M_a = (double *)malloc(rows_a * cols_a * sizeof(double));
    double *M_b = (double *)malloc(rows_b * cols_b * sizeof(double));
    double *M_c = NULL;
    double *M_d = NULL;

    if (!M_a || !M_b)
    {
        fprintf(stderr, "Couldn't allocate memory!\n");
        exit(EXIT_FAILURE);
    }

    init_rand(M_a, rows_a, cols_a);
    init_rand(M_b, rows_b, cols_b);

    // ROI -> Se multiplican
    M_c = mat_mul_slow(M_a, rows_a, cols_a, M_b, rows_b, cols_b);

    // print(M_a, rows_a, cols_a);
    // printf("Array A: %s\n", arr_to_str(M_a, rows_a * cols_a));
    // printf("Array B: %s\n", arr_to_str(M_b, rows_b * cols_b));
    // printf("Array C: %s\n", arr_to_str(M_c, rows_a * cols_b));

    free(M_a);
    free(M_b);
    free(M_c);
    free(M_d);

    return EXIT_SUCCESS;
}