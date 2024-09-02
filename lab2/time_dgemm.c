#include "stdio.h"
#include "stdlib.h"
#include "sys/time.h"
#include "time.h"
#include <cblas.h>
#include "math.h"

#define TOLERANCE 1e-7

void dgemm_naive(int m, int n, int k, double alpha,double beta,
          double *A, double *B, double *C,int lda,int ldb,int ldc){
    for(int i=0; i< m;i ++){    //C[i] 
        for(int j=0; j< n; j++){  //C[i][j]
            C[i*ldc+j] = beta*C[i*ldc+j];
            for(int p=0; p< k; p++){  
                C[i*ldc+j] += A[i*lda+p]*B[p*ldb+j]*alpha; 
             }
        }
    }
}

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    printf("Input Error\n");
    return 1;
  }

  printf("test!\n");
  int i, m, n, k;
  m = n = k = atoi(argv[1]);

  int sizeofa = m * k;
  int sizeofb = k * n;
  int sizeofc = m * n;
  int lda = m;
  int ldb = k;
  int ldc = m;

  double alpha = 1.2;
  double beta = 0.001;

  struct timeval start, finish,start_naive,finish_naive;
  double duration,duration_naive;

  double *A = (double *)malloc(sizeof(double) * sizeofa);
  double *B = (double *)malloc(sizeof(double) * sizeofb);
  double *C_cblas = (double *)malloc(sizeof(double) * sizeofc);
  double *C_naive = (double *)malloc(sizeof(double) * sizeofc);  
  srand((unsigned)time(NULL));

  for (i = 0; i < sizeofa; i++)
  {
    A[i] = i % 3 + 1; // (rand() % 100) / 100.0;
  }

  for (i = 0; i < sizeofb; i++)
  {
    B[i] = i % 3 + 1; //(rand()%100)/10.0;
  }

  for (i = 0; i < sizeofc; i++)
  {
    C_cblas[i] = 0.1;
    C_naive[i] = 0.1;
  }


  printf("m=%d,n=%d,k=%d,alpha=%lf,beta=%lf,sizeofc=%d\n", m, n, k, alpha, beta, sizeofc);
  gettimeofday(&start, NULL);
  cblas_dgemm(CblasColMajor, CblasNoTrans, CblasNoTrans, m, n, k, alpha, A, lda, B, ldb, beta, C_cblas, ldc);
  gettimeofday(&finish, NULL);

  gettimeofday(&start_naive, NULL);
  dgemm_naive(m,n,k,alpha,beta,A,B,C_naive,lda,ldb,ldc);
  gettimeofday(&finish_naive, NULL);

  // 转成成秒数
  duration = (double)(finish.tv_sec - start.tv_sec) + (double)(finish.tv_usec - start.tv_usec) / 1.0e6;
  double gflops = 4.0 * m * n * k;
  gflops = gflops / duration * 1.0e-9;
  duration_naive = (double)(finish_naive.tv_sec - start_naive.tv_sec) + (double)(finish_naive.tv_usec - start_naive.tv_usec) / 1.0e6;
  double gflops_naive = 4.0 * m * n * k;
  gflops_naive = gflops_naive / duration_naive * 1.0e-9;


 //结果对比
 for(int i=0;i<=m*n;i++){
    if(fabs(C_cblas[i]-C_naive[i])>TOLERANCE){
        printf("The results are different!\n");
        break;
    }
    if(i == m*n){
        printf("The results are same!\n");
    }
 }


  FILE *fp;
  fp = fopen("timeDGEMM.txt", "a"); // 追加写
  fprintf(fp, "cblas %dx%dx%d\t%lf s\t%lf GFLOPS\n", m, n, k, duration, gflops);
  fprintf(fp, "naive %dx%dx%d\t%lf s\t%lf GFLOPS\n", m, n, k, duration_naive, gflops_naive);
  fclose(fp);

  free(A);
  free(B);
  free(C_cblas);
  free(C_naive);
  return 0;
}
