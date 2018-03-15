#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include <time.h>
#include <assert.h>
void free_mat(int ** mat, int rows);
int * convert_matrix_to_array(int rows, int cols, int * mat[cols]);
int ** create_matrix(int rows, int cols);
void print_row(int * row, int length);
float average(int * row, int cols);

int main(int argc, char ** argv) {
    int cols = 10000;
    int rows = 10000;

    MPI_Init(NULL, NULL);

    srand(time(NULL));

    int n_proc;
    MPI_Comm_size(MPI_COMM_WORLD, &n_proc);

    int my_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    MPI_Barrier(MPI_COMM_WORLD);
    clock_t t;
    t = clock();
    float * partial_avgs = NULL;

    /* Important, get back to it later */
    int * mat_array = NULL;
    int ** MAT = NULL;

    if (my_rank == 0) {
        MAT = create_matrix(rows, cols);
        mat_array = convert_matrix_to_array(rows, cols, MAT);
        partial_avgs = (float *) malloc(n_proc * sizeof(float));
    }

    int numbers_per_process = (cols * rows) / n_proc;
    /* each process stores their row in row_data */
    int * row_data = (int *) malloc(numbers_per_process * sizeof(int));
    

    printf("Process 0 will get: %d\n", numbers_per_process);
    MPI_Scatter(mat_array, numbers_per_process, MPI_INT, 
            row_data, numbers_per_process, MPI_INT, 0, MPI_COMM_WORLD);

    float row_avg = average(row_data, numbers_per_process);

    int size_of_avg = 1;

    /* count of elements received per process */
    int size_of_data_to_receive = 1;
    
    int root_id = 0;

    MPI_Gather(&row_avg, size_of_avg, MPI_FLOAT, partial_avgs, size_of_data_to_receive, MPI_FLOAT, root_id, MPI_COMM_WORLD);
    
    if (my_rank == 0) {
        float sum = 0.0;
        for (int i = 0; i < n_proc; i++) {
            sum += partial_avgs[i];
            printf("AVG from process %d: %.2f\n", i, partial_avgs[i]);
        }

        float total_avg = sum / n_proc;
        printf("Total Matrix Average: %.4f.\n", total_avg);

        free(mat_array);
        free_mat(MAT, n_proc);
    }

    free(row_data);
    free(partial_avgs);
    MPI_Barrier(MPI_COMM_WORLD);

    if (my_rank == 0) {
        t = clock() - t;
        double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds)))
        
        printf("Time elapsed: %f\n", time_taken);
    }
    
    MPI_Finalize();

}

void free_mat(int ** mat, int rows) {
    for (int i = 0; i < rows; i++) {
        free(mat[i]);
    }
}

int ** create_matrix(int rows, int cols) {

    int ** MAT = (int **) malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; i++) {
        MAT[i] = (int *) malloc(cols * sizeof(int));
    }

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            MAT[row][col] = rand() % 1000;
        }
    }

    return MAT;
}

int * convert_matrix_to_array(int rows, int cols, int * mat[cols] ) {
    int * new_mat = (int *) malloc(rows * cols * sizeof(int));

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            new_mat[(i * cols) + j] = mat[i][j];
        }
    }

    return new_mat;
}

float average(int * row, int cols) {
    double sum = 0.0;
    for (int i = 0; i < cols; i++) {
        sum += row[i];
    }

    double avg = (double)sum / (double)cols;
    return avg;
}

void print_row(int * row, int length) {
    for (int i = 0; i < length; i++) {
        printf("%d, ", row[i]);
    }
    
    printf("\n");
}
