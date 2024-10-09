#include "defs.h"

void MY_MMult(int m, int n, int k, double *a, int lda,
              double *b, int ldb,
              double *c, int ldc)
{
  int i, j, p;

  for (i = 0; i < m; i++) 
  {
    for (p = 0; p < k; p++) 
    {
      for (j = 0; j < k; j++)
      { 
        C(i, j) += A(i, p) * B(p, j);
      }
    }
  }
}
