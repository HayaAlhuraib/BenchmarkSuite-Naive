/* Standard C includes */
#define _GNU_SOURCE
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <sched.h>
#include <stdbool.h>
#include <inttypes.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>

/* Include all implementations declarations */
#include "impl/naive.h"  // Updated for mmult naive implementation

/* Include common headers */
#include "common/types.h"
#include "common/macros.h"

/* Include application-specific headers */
#include "include/types.h"

const int SIZE_DATA = 4 * 1024 * 1024;

int main(int argc, char** argv) {
    /* Set the buffer for printf to NULL */
    setbuf(stdout, NULL);

    /* Arguments */
    int nthreads = 1;
    int cpu      = 0;

    int nruns    = 10000;
    int nstdevs  = 3;

    /* Data */
    int data_size = SIZE_DATA;

    /* Parse arguments */
    void* (*impl_scalar_naive_ptr)(void* args) = impl_scalar_naive;  // Updated

    /* Chosen */
    void* (*impl)(void* args) = NULL;
    const char* impl_str      = NULL;

    bool help = false;
    for (int i = 1; i < argc; i++) {
        /* Implementations */
        if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--impl") == 0) {
            assert(++i < argc);
            if (strcmp(argv[i], "naive") == 0) {
                impl = impl_scalar_naive_ptr; impl_str = "scalar_naive";
            } else {
                impl = NULL; impl_str = "unknown";
            }
            continue;
        }

        /* Input/output data size */
        if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--size") == 0) {
            assert(++i < argc);
            data_size = atoi(argv[i]);
            continue;
        }
    }

    if (help || impl == NULL) {
        printf("Usage: %s {-i | --impl} naive [Options]\n", argv[0]);
        printf("  Options:\n");
        printf("    -h | --help      Print this message\n");
        printf("    -s | --size      Size of input matrices (default = %d)\n", data_size);
        exit(help ? 0 : 1);
    }
    srand((unsigned int)time(NULL)); // Seed the random number generator
    /* Initialize matrices */
    size_t size = data_size;  // Square matrix dimensions
    float* A = malloc(size * size * sizeof(float));
    float* B = malloc(size * size * sizeof(float));
    float* R = malloc(size * size * sizeof(float));

    if (!A || !B || !R) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1);
    }

    /* Initialize input matrices */
    for (size_t i = 0; i < size * size; i++) {
        A[i] = (float)(rand() % 10);
        B[i] = (float)(rand() % 10);
    }

    /* Prepare arguments */
    args_t args = {
        .input = (void*)A,
        .output = (void*)R,
        .size = size,
        .cpu = cpu,
        .nthreads = nthreads
    };

    /* Run the implementation */
    printf("Running %s implementation:\n", impl_str);
    clock_t start = clock();
    (*impl)((void*)&args);
    clock_t end = clock();
    double naive_time = (double)(end - start) / CLOCKS_PER_SEC;
    /* Display results (optional) */
    printf("matrix A:\n");
    for (size_t i = 0; i < size; i++) {
        for (size_t j = 0; j < size; j++) {
            printf("%.2f ", A[i * size + j]);
        }
        printf("\n");
    }
   printf("matrix B:\n");
    for (size_t i = 0; i < size; i++) {
        for (size_t j = 0; j < size; j++) {
            printf("%.2f ", B[i * size + j]);
        }
        printf("\n");
    }
    /* Display results (optional) */
    printf("Result matrix R:\n");
    for (size_t i = 0; i < size; i++) {
        for (size_t j = 0; j < size; j++) {
            printf("%.2f ", R[i * size + j]);
        }
        printf("\n");
    }
   printf("Naive Runtime: %.6f seconds\n", naive_time);
    /* Free memory */
    free(A);
    free(B);
    free(R);

    return 0;
}
