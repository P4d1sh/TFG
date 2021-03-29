#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "matrix.h"

char *arr_to_str(const double *M, const unsigned rows, const unsigned cols)
{
    size_t i, j;
    // #digitos maximos dentro de un numero = 18
    int max_digit_in_num = 18;
    char *str, *aux;
    str = (char *)malloc(max_digit_in_num * sizeof(char) * rows * cols);
    aux = (char *)malloc(max_digit_in_num * sizeof(char));
    strcpy(str, "\n");
    for (i = 0; i < rows; i++)
    {
        strcat(str, "|\t");
        for (j = 0; j < cols; j++)
        {
            sprintf(aux, "%8.3f", M[i * cols + j]);
            if (j < cols - 1)
            {
                strcat(aux, "\t");
            }
            else if (j < cols)
            {
                strcat(aux, "\t|\n");
            }
            strcat(str, aux);
        }
    }
    free(aux);
    return str;
}

void init_rand(double *M, const unsigned rows, const unsigned cols)
{
    size_t i, j;
    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < cols; j++)
        {
            M[i * cols + j] = (double)(rand()) / ((double)RAND_MAX) * MAX_RANDOM;
        }
    }
}

void init_seq(double *M, const unsigned rows, const unsigned cols)
{
    size_t i, j;
    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < cols; j++)
        {
            M[i * cols + j] = i * cols + j;
        }
    }
}

double *mat_mul(const double *M_a,
                const unsigned rows_a, const unsigned cols_a,
                const double *M_b,
                const unsigned rows_b, const unsigned cols_b)
{
    size_t i, j, k;

    if (cols_a != rows_b)
    {
        fprintf(stderr, "#columns A must be equal to #rows B!\n");
        exit(EXIT_FAILURE);
    }

    double *M_c = (double *)malloc(rows_a * cols_b * sizeof(double));
    if (M_c == NULL)
    {
        fprintf(stderr, "Couldn't allocate memory!\n");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < rows_a; i++)
    {
        for (k = 0; k < cols_b; k++)
        {
            double sum = 0.0;
            for (j = 0; j < cols_a; j++)
            {
                sum += M_a[i * cols_a + j] * M_b[j * cols_b + k];
            }
            M_c[i * cols_b + k] = sum;
        }
    }

    return M_c;
}

double *mat_mul_multithread(const double *M_a,
                            const unsigned rows_a, const unsigned cols_a,
                            const double *M_b,
                            const unsigned rows_b, const unsigned cols_b)
{
    // // TODO: Let's start with square matrices (nxn)
    // size_t i, j, k;

    // if (cols_a != rows_b)
    // {
    //     fprintf(stderr, "#columns A must be equal to #rows B!\n");
    //     exit(EXIT_FAILURE);
    // }

    // double *M_b_Tr = (double *)malloc(rows_b * cols_b * sizeof(double));
    // double *M_c = (double *)malloc(rows_a * cols_b * sizeof(double));
    // if (M_c == NULL || M_b_Tr == NULL)
    // {
    //     fprintf(stderr, "Couldn't allocate memory!\n");
    //     exit(EXIT_FAILURE);
    // }

    // // Declaring the threads
    // pthread_t *threads;
    // threads = (pthread_t *)malloc(NUM_THREADS * sizeof(pthread_t));

    // // Creating four threads, each evaluating its own part
    // for (i = 0; i < NUM_THREADS; i++)
    // {
    //     int *p;
    //     pthread_create(&threads[i], NULL, __multi, (void *)(p));
    // }

    // // joining and waiting for all threads to complete
    // for (int i = 0; i < NUM_THREADS; i++)
    // {
    //     pthread_join(threads[i], NULL);
    // }

    // return M_c;
    return NULL;
}

void *__multi(void *arg)
{
    // int core = step_i++;

    // // Each thread computes 1/4th of matrix multiplication
    // for (int i = core * MAX / 4; i < (core + 1) * MAX / 4; i++)
    //     for (int j = 0; j < MAX; j++)
    //         for (int k = 0; k < MAX; k++)
    //             matC[i][j] += matA[i][k] * matB[k][j];
    return NULL;
}

double *mat_mul_transpose(const double *M_a,
                          const unsigned rows_a, const unsigned cols_a,
                          const double *M_b,
                          const unsigned rows_b, const unsigned cols_b)
{
    size_t i, j, k;

    if (cols_a != rows_b)
    {
        fprintf(stderr, "#columns A must be equal to #rows B!\n");
        exit(EXIT_FAILURE);
    }

    double *M_b_Tr = (double *)malloc(rows_b * cols_b * sizeof(double));
    double *M_c = (double *)malloc(rows_a * cols_b * sizeof(double));
    if (M_c == NULL || M_b_Tr == NULL)
    {
        fprintf(stderr, "Couldn't allocate memory!\n");
        exit(EXIT_FAILURE);
    }

    // Calculate the transpose
    for (i = 0; i < rows_b; i++)
    {
        for (j = 0; j < cols_b; j++)
        {
            M_b_Tr[j * rows_b + i] = M_b[i * cols_b + j];
        }
    }

    for (i = 0; i < rows_a; i++)
    {
        for (k = 0; k < cols_b; k++)
        {
            double sum = 0.0;
            for (j = 0; j < cols_a; j++)
            {
                sum += M_a[i * cols_a + j] * M_b_Tr[k * rows_b + j];
            }
            M_c[i * cols_b + k] = sum;
        }
    }
    free(M_b_Tr);
    return M_c;
}
