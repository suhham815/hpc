#include "defs.h"

void MY_MMult(int m, int n, int k, double *a, int lda,
              double *b, int ldb,
              double *c, int ldc)
{
  int i, j, p;

  for (j = 0; j < k; j++)
  {
    for (p = 0; p < k; p++) 
    {
      for (i = 0; i < m; i++)
      { 
        C(i, j) += A(i, p) * B(p, j);
      }
    }
  }
}
