#include <stdio.h>
#include "mpi.h"

int main(int argc, char ** argv) {
    MPI_Init(&argc, &argv);
    int world_size, my_world_rank;

    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_world_rank);
    int mat[4][4] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

    if (my_world_rank == 0) {
        for (int i = 0; i < 4; i++) {
            printf("| ");
            for (int j = 0; j < 4; j++) {
                printf("%5d |", mat[i][j]);
            }
            printf("\n");
        }
        printf("\n");
    }
    MPI_Barrier(MPI_COMM_WORLD);
    printf("My world rank: %d\n", my_world_rank);

    int size = 4;
    int ranks[size];

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (i==j)
                ranks[i] = i*size + j;
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);
    //Splitting MPI_COMM_WORLD into groups
    MPI_Group world_group;
    MPI_Comm_group(MPI_COMM_WORLD, &world_group);

    MPI_Comm diagonal_comm;
    MPI_Group diagonal_group;
    MPI_Group_incl(world_group, size, ranks, &diagonal_group);
    
    MPI_Comm_create_group(MPI_COMM_WORLD, diagonal_group, 0, &diagonal_comm);


    
    // Check members of the newly created group
    int diagonal_rank = -1, diagonal_size = -1;

    if (MPI_COMM_NULL != diagonal_comm) {
        MPI_Comm_rank(diagonal_comm, &diagonal_rank);
        MPI_Comm_size(diagonal_comm, &diagonal_size);
    }

    printf("MPI_COMM_WORLD RANK: %d --- DIAGONAL RANK: %d\n",
        my_world_rank, diagonal_rank);

    MPI_Group_free(&world_group);
    MPI_Group_free(&diagonal_group);
    if (MPI_COMM_NULL != diagonal_comm)
        MPI_Comm_free(&diagonal_comm);

    MPI_Finalize();
}
