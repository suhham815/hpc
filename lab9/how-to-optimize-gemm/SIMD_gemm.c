#include "defs.h"
#include <immintrin.h>  

void MY_MMult(int m, int n, int k, double *a, int lda,
              double *b, int ldb,
              double *c, int ldc)
{
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            __m256d sum = _mm256_setzero_pd(); 

            for (int p = 0; p < k; p += 4) {
                
                double b_data[4];
                for(int k = 0; k < 4; k++){
                    b_data[k] = B(p+k,j);
                }
                __m256d a_part = _mm256_loadu_pd(&A(i,p));
                __m256d b_part = _mm256_loadu_pd(b_data);

                __m256d ab = _mm256_mul_pd(a_part, b_part);
                sum = _mm256_add_pd(sum,ab);
            }

            double result[4];
            _mm256_storeu_pd(result, sum); 

            C(i, j) += result[0] + result[1] + result[2] + result[3];
        }
    }
}