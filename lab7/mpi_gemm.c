#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

 #define matrix_size 8

int main(int argc, char *argv[]) {
    int myid, numprocs;
    int m, n, k;
    m = n = k = matrix_size;
    int row;
    double *A, *B, *C;
    double *a, *c;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

    
    row = m / numprocs;

    //初始化A，B，C 
    B = (double *)malloc(k * n * sizeof(double));
    if (myid == 0) {
        
        A = (double *)malloc(m * k * sizeof(double));
        C = (double *)malloc(m * n * sizeof(double));

        double A_v[] = {1.900078e-02, 8.548636e-02, -3.314322e-01, 2.440604e-02, -6.461445e-01, -7.460251e-01, 5.706861e-01, -6.391164e-02, 
                        -3.681220e-01, 3.359572e-01, -8.358069e-01, 1.730207e-01, -7.762620e-01, -9.918196e-01, -1.155304e-01, -6.060053e-01, 
                        4.034051e-01, -6.577163e-01, -4.593707e-01, 4.775171e-01, 7.053617e-01, -9.989944e-01, -4.241922e-01, -1.056461e-01, 
                        -2.655067e-01, 6.615583e-01, -8.514753e-01, 7.433776e-01, 6.223437e-01, -6.140962e-01, 7.067059e-01, 4.568796e-01, 
                        7.526247e-01, 7.238108e-01, 3.524515e-01, 1.559346e-01, 9.097746e-01, 1.735603e-01, 5.194394e-02, 9.321860e-01, 
                        -2.580107e-01, 5.384423e-01, 6.203171e-01, 9.294072e-01, 8.085664e-01, -2.500817e-01, 4.568334e-01, 5.136816e-01, 
                        5.050480e-02, -8.948411e-01, -7.157754e-02, -3.329414e-01, -7.221695e-01, -9.464281e-01, -4.673714e-01, -6.788623e-02, 
                        -1.443309e-01, 9.334593e-01, 1.383701e-01, -2.143441e-01, 3.723747e-01, -7.046941e-02, 5.669680e-01, 7.379329e-01};
        double B_v[] = {-7.319215e-01, 8.165651e-01, -4.029224e-02, 8.852518e-01, 5.361704e-01, -2.435351e-01, -5.578819e-01, -3.161686e-01, 
                        6.921052e-01, 5.488294e-01, -9.377434e-01, -2.239507e-01, 5.038825e-01, -1.625671e-01, 5.086909e-02, -8.637625e-02, 
                        -9.638247e-01, -2.678706e-01, 2.168604e-01, 8.610245e-01, -8.182718e-01, -7.231037e-01, -1.094956e-02, 1.762435e-01, 
                        8.817096e-01, 8.885760e-01, 5.845223e-01, 8.227684e-01, 7.079334e-01, -2.383066e-01, 7.650440e-01, 1.702999e-01, 
                        3.401642e-01, 7.696433e-01, -6.350064e-01, -3.245633e-01, -6.507111e-01, -4.004399e-01, 5.476602e-01, -8.601459e-01, 
                        5.999431e-01, -1.096578e-01, 5.024259e-02, 6.562465e-02, -9.604593e-01, -9.579758e-01, -8.819950e-01, 6.378629e-01, 
                        -7.135664e-01, -6.739431e-01, 7.975177e-01, 4.041545e-01, -1.618930e-01, -5.223948e-01, -7.603972e-01, -1.936786e-01, 
                        2.209206e-01, -1.503654e-01, -3.824202e-01, 9.420213e-01, -4.072602e-01, -9.257813e-01, 9.560251e-02, 1.709661e-01};
        for (int i = 0; i < m * k; i++) 
        {
            A[i] = A_v[i];
        }
        for (int i = 0; i < k * n; i++) 
        {
            B[i] = B_v[i];
        }
        for (int i = 0; i < m * n; i++) 
        {
            C[i] = 0.0;
        }
    }
    
    a = (double *)malloc(row * k * sizeof(double));
    MPI_Scatter(A, row * k, MPI_DOUBLE, a, row * k, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
    MPI_Bcast(B, k * n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    c = (double *)malloc(row * n * sizeof(double));
    for (int i = 0; i < row * n; i++) 
    {
        c[i] = 0.0;
    }

    for(int i=0; i<row; i++)
    {   
    	for(int j = 0; j<n; j++)
        {
        	for(int k = 0; k<n; k++)
            		{
                        c[i*n+j] += a[i*n+k] * B[k*n+j];
                    }
        }
    }

    MPI_Gather(c, row * n, MPI_DOUBLE, C, row * n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (myid == 0) {
        printf("Matrix size:%d*%d\n", matrix_size, matrix_size);
        printf("Matrix C:\n");
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                printf("%lf ", C[i*n + j]);
            }
            printf("\n");
        }
        free(A);
        free(B);
        free(C);
    }

    free(a);
    free(c);

    MPI_Finalize();
    return 0;
}