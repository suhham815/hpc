//分块后数据重排
#include "defs.h"
#include <stdlib.h>

void PackMatrixA(int m, int k, double *a, int lda, double *A_block);
void PackMatrixB(int k, int n, double *b, int ldb, double *B_block);
void InnerKernel(int m, int n, int k, double *A_block, double *B_block, double *c, int ldc);

void MY_MMult(int m, int n, int k, double *a, int lda,
              double *b, int ldb,
              double *c, int ldc) 
{
    int blocksize = 64;
    double *A_block = (double *)malloc(blocksize * blocksize * sizeof(double));
    double *B_block = (double *)malloc(blocksize * blocksize * sizeof(double));

    for (int i = 0; i < m; i += blocksize) {
        for (int j = 0; j < n; j += blocksize) {
            for (int p = 0; p < k; p += blocksize) {
                int ib = (i + blocksize > m) ? m - i : blocksize;
                int jb = (j + blocksize > n) ? n - j : blocksize;
                int pb = (p + blocksize > k) ? k - p : blocksize;

                PackMatrixA(ib, pb, &A(i, p), lda, A_block);
                PackMatrixB(pb, jb, &B(p, j), ldb, B_block);
                InnerKernel(ib, jb, pb, A_block, B_block, &C(i, j), ldc);
            }
        }
    }

    free(A_block);
    free(B_block);
}

void PackMatrixA(int m, int k, double *a, int lda, double *A_block) 
{
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < k; j++) {
            A_block[i * k + j] = a[i * lda + j];
        }
    }
}

void PackMatrixB(int k, int n, double *b, int ldb, double *B_block) 
{
    for (int j = 0; j < k; j++) {
        for (int i = 0; i < n; i++) {
            B_block[i * k + j] = b[i + j * ldb];
        }
    }
}

void InnerKernel(int m, int n, int k, double *A_block, double *B_block, double *c, int ldc) 
{
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            double sum = 0.0;
            for (int p = 0; p < k; p++) {
                sum += A_block[i * k + p] * B_block[j * k + p];
            }
            C(i,j) += sum;
        }
    }
}