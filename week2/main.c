//#include "mpi.h"
/* printf, fopen, fclose, fscanf */
#include <stdio.h>

/* EXIT_SUCCESS, malloc, free */
#include <stdlib.h>

/* rand */
#include <time.h>

/* getopt */
#include <unistd.h>
int * create_value_matrix(int rows, int cols);
int * create_empty_matrix(int rows, int cols);
void serial_mat_vec_multiply(int * A, int * B, int * C, int rowsA, int colsA, int rowsB, int colsB);
void parallel_mat_vec_multiply(int * A, int * B, int * C, int rowsA, int colsA, int rowsB, int colsB);

int main(int argc, char ** argv) {
    //MPI_Init(&argc, &argv);
    int opt, num_threads;
    size_t rows1, rows2, cols1, cols2;
    num_threads = 1;

    while (-1 != (opt=getopt(argc, argv, "i:j:k:l:t:"))) {
        switch (opt) {
            case 't':
            num_threads = atoi(optarg);
            break;
      
            case 'i':
            rows1 = (size_t)atoi(optarg);
            break;

            case 'j':
            cols1 = (size_t)atoi(optarg);
            break;

            case 'k':
            rows2 = (size_t)atoi(optarg);
            break;

            case 'l':
            cols2 = (size_t)atoi(optarg);
            break;

            default: /* '?' */
            fprintf(stderr, "Usage: %s [-i mat1-rows] [-j mat1-cols] "\
                "[-k mat2-rows] [-l mat2-cols] [-t num_threads]\n", argv[0]);
            return EXIT_FAILURE;
        }
    }
    
    if (cols1 != rows2) {
        fprintf(stderr, "Matrix dimension mismatch.");
        return EXIT_FAILURE;
    }
    
    int * A = create_value_matrix(rows1, cols1);
    int * B = create_value_matrix(rows2, cols2);
    int * C = create_empty_matrix(cols1, rows2);
    
    /* 1 thread, Mat x vector multiply */
    if (num_threads == 1 && cols2 == 1) {
        clock_t start_time = clock();
        serial_mat_vec_multiply(A, B, C, rows1, cols1, rows2, cols2);

        clock_t end_time = clock();
        double elapsed = ((double)(end_time - start_time));
        printf("Execution time: %f sec.\n", elapsed/CLOCKS_PER_SEC);
    }

    /* parallel matrix x vector multiply */
    if (num_threads == rows1) {
        
    }
    //MPI_Finalize();
    free(A);
    free(B);
    free(C);
    return 0;
}

int * create_value_matrix(int rows, int cols) {
    srand(time(0));

    int * matrix = (int *) malloc(rows * cols * sizeof(int));
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i * cols + j] = rand() % 10 + 1;
        }
    }

    return matrix;
}

int * create_empty_matrix(int rows, int cols) {
    srand(time(0));

    int * matrix = (int *) malloc(rows * cols * sizeof(int));

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i * cols + j] = 0;
        }
    }

    return matrix;
}

void serial_mat_vec_multiply(int * A, int * B, int * C,
        int rowsA, int colsA, int rowsB, int colsB) {
   for (int i = 0; i < rowsA; i++) {
       for (int j = 0; j < colsA; j++) {
            C[i] = C[i] + A[i * rowsA + j] * B[j];
       }
   }
   printf("Computation complete.\n");
}

void parallel_mat_vec_multiply(int * A, int * B, int * C, int rowsA, int colsA, int rowsB, int colsB) {
    
}
