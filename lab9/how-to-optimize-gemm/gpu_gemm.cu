#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>
#include <cublas_v2.h>
#include "defs.h"

extern "C" void MY_MMult(int m, int n, int k, double *a, int lda,
              double *b, int ldb,
              double *c, int ldc) 
{
    double *d_a, *d_b, *d_c; 
    cudaError_t err;

    err = cudaMalloc((void**)&d_a, lda * m * sizeof(double));
    if (err != cudaSuccess) {
        fprintf(stderr, "CUDA Error allocating d_a: %s\n", cudaGetErrorString(err));
        return;
    }
    err = cudaMalloc((void**)&d_b, ldb * k * sizeof(double));
    if (err != cudaSuccess) {
        fprintf(stderr, "CUDA Error allocating d_a: %s\n", cudaGetErrorString(err));
        return;
    }
    err = cudaMalloc((void**)&d_c, ldc * m * sizeof(double));
    if (err != cudaSuccess) {
        fprintf(stderr, "CUDA Error allocating d_a: %s\n", cudaGetErrorString(err));
        return;
    }

    cudaMemcpy(d_a, a, lda * k * sizeof(double), cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, b, ldb * n * sizeof(double), cudaMemcpyHostToDevice);
    cudaMemcpy(d_c, c, ldb * n * sizeof(double), cudaMemcpyHostToDevice);

    cublasHandle_t handle;
    cublasCreate(&handle);

    double alpha = 1.0;
    double beta = 1.0;

    cublasDgemm(handle, CUBLAS_OP_N, CUBLAS_OP_N, 
                m, n, k, 
                &alpha, 
                d_a, lda, 
                d_b, ldb, 
                &beta, 
                d_c, ldc);

    cudaMemcpy(c, d_c, ldc * n * sizeof(double), cudaMemcpyDeviceToHost);

    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);
    cublasDestroy(handle);
}