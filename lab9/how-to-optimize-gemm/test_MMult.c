#include <stdio.h>
#include <stdlib.h>
#include <cblas.h>
#include "defs.h"

void MY_MMult(int, int, int, double *, int, double *, int, double *, int);
void copy_matrix(int, int, double *, int, double *, int);
void random_matrix(int, int, double *, int);
void zero_matrix(int, int, double *, int);
double compare_matrices(int, int, double *, int, double *, int);
void print_rowmajor_matrix(int m, int n, double *a, int lda);
double dclock();
void REF_MMult(int, int, int, double *, int, double *, int, double *, int);

int main()
{
  int
      p,
      m, n, k,
      lda, ldb, ldc,
      rep;

  double
      dtime,
      dtime_best,
      diff,
      gflops;

  double
      *a,
      *b, *c,
      *cref,
      *cold; // Random Initialization value

  printf("MY_MMult = [\n");

  for (p = PFIRST; p <= PLAST; p = 2 * p)
  {
    m = (M == -1 ? p : M);
    n = (N == -1 ? p : N);
    k = (K == -1 ? p : K);

    gflops = 2.0 * m * n * k * 1.0e-09;


    lda = (LDA == -1 ? k : LDA);
    ldb = (LDB == -1 ? n : LDB);
    ldc = (LDC == -1 ? n : LDC);

    /* Allocate space for the matrices */
    /* Note: I create an extra column in A to make sure that
       prefetching beyond the matrix does not cause a segfault */
    a = (double *)malloc(lda * (k + 1) * sizeof(double));
    b = (double *)malloc(ldb * n * sizeof(double));
    c = (double *)malloc(ldc * n * sizeof(double));
    cold = (double *)malloc(ldc * n * sizeof(double));
    cref = (double *)malloc(ldc * n * sizeof(double));

    /* Generate random matrices A, B, Cold */
    random_matrix(m, k, a, lda);
    random_matrix(k, n, b, ldb);
    random_matrix(m, n, cold, ldc);

    copy_matrix(m, n, cold, ldc, cref, ldc);
    REF_MMult(m, n, k, a, lda, b, ldb, cref, ldc);

    /* Time the "optimized" implementation */
    for (rep = 0; rep < NREPEATS; rep++)
    {
      copy_matrix(m, n, cold, ldc, c, ldc);

      /* Time your implementation */
      dtime = dclock();
      MY_MMult(m, n, k, a, lda, b, ldb, c, ldc);
      dtime = dclock() - dtime;

      if (rep == 0)
        dtime_best = dtime;
      else
        dtime_best = (dtime < dtime_best ? dtime : dtime_best);
    }

    diff = compare_matrices(m, n, c, ldc, cref, ldc);

    printf("%d %le %le\n", p, gflops / dtime_best, diff);
    fflush(stdout);

    free(a);
    free(b);
    free(c);
    free(cold);
  }

  printf("];\n");

  exit(0);
}
