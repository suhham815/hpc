#include "defs.h"

void MY_MMult(int m, int n, int k, double *a, int lda,
              double *b, int ldb,
              double *c, int ldc)
{
    int blocksize = 64; 
    for (int i = 0; i < m; i += blocksize) {
        for (int j = 0; j < n; j += blocksize) {
            for (int p = 0; p < k; p += blocksize) {
                
                int i_max = (i + blocksize < m) ? i + blocksize : m;
                int j_max = (j + blocksize < n) ? j + blocksize : n;
                int p_max = (p + blocksize < k) ? p + blocksize : k;

                for (int ii = i; ii < i_max; ii++) {
                    for (int jj = j; jj < j_max; jj++) {
                        double sum = 0.0;
                        for (int pp = p; pp < p_max; pp++) {
                            sum += A(ii,pp) * B(pp,jj); 
                        }
                        C(ii,jj) += sum;
                    }
                }
            }
        }
    }
}
