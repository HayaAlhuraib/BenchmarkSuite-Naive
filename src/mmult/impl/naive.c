/* Standard C includes */
#include <stdlib.h>

/* Include common headers */
#include "common/macros.h"
#include "common/types.h"

/* Include application-specific headers */
#include "include/types.h"

/* Naive Implementation */
#pragma GCC push_options
#pragma GCC optimize ("O1")
void* impl_scalar_naive(void* args) {
    /* Extract arguments */
    args_t* arguments = (args_t*)args;
    size_t size = arguments->size;

    byte* A = (byte*)arguments->input;
    byte* B = (byte*)(arguments->input + size * size * sizeof(float));
    byte* R = (byte*)arguments->output;

    /* Perform matrix-matrix multiplication */
    for (size_t i = 0; i < size; i++) {
        for (size_t j = 0; j < size; j++) {
            R[i * size + j] = 0.0f;
            for (size_t k = 0; k < size; k++) {
                R[i * size + j] += A[i * size + k] * B[k * size + j];
            }
        }
    }

    return NULL;
}
#pragma GCC pop_options
