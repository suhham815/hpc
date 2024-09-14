#include "defs.h"
#include <pthread.h>

typedef struct {
    int start_row;
    int end_row;
    double* A;
    double* B;
    double* C;
    int m;
    int n;
} ThreadData;


void* matrix_multiply(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    int start_row = data->start_row;
    int end_row = data->end_row;
    double* A = data->A;
    double* B = data->B;
    double* C = data->C;
    int n = data->n;

    for (int i = start_row; i < end_row; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                C[i*n+j] = C[i*n+j] + A[i*n+k] * B[k*n+j];
            }
        }
    }
    pthread_exit(NULL);
}

void MY_MMult(int m, int n, int k, double *a, int lda,
              double *b, int ldb,
              double *c, int ldc)
{
    int rows_per_thread = m / NUM_THREADS;
    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];
 
     for (int i = 0; i < NUM_THREADS;i++) {
            thread_data[i].start_row = i * rows_per_thread;
            thread_data[i].end_row = (i + 1) * rows_per_thread;
            thread_data[i].A = a;
            thread_data[i].B = b;
            thread_data[i].C = c;
            thread_data[i].n = n;
            pthread_create(&threads[i], NULL, matrix_multiply, (void*)&thread_data[i]);
        }
 
        for (int i = 0; i < NUM_THREADS; i++) {
            pthread_join(threads[i], NULL);
        }
}
