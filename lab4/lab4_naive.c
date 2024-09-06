#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N 1024

void naive_dgemm(double *A, double *B, double *C){
    for(int i=0; i< N;i ++){ 
        for(int j=0; j< N; j++){  
            for(int k=0; k< N; k++){  
                C[i*N+j] += A[i*N+k]*B[k*N+j]; 
             }
        }
    }
}

int main()
{
    int sizeofa = N*N;
    int sizeofb = N*N;
    int sizeofc = N*N;


    //初始化矩阵
    double *A = (double *)malloc(sizeof(double) * sizeofa);
    double *B = (double *)malloc(sizeof(double) * sizeofb);
    double *C = (double *)malloc(sizeof(double) * sizeofc);  

    for (int i = 0; i < sizeofa; i++)
    {
        A[i] = i % 3 + 1; 
     }

     for (int i = 0; i < sizeofb; i++)
    {
        B[i] = i % 3 + 1; 
    }

    for (int i = 0; i < sizeofc; i++)
    {
        C[i] = 0.1;
    }

    int rc = fork();
    if (rc < 0) {
        // fork failed; exit
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        printf("Child process.\n");
        naive_dgemm(A,B,C);

    } else {
        printf("Parent process.\n");
        naive_dgemm(A,B,C);
    }
    return 0;

}