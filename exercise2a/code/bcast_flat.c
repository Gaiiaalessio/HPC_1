#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void my_flat_tree_broadcast(int *my_data, int my_rank, int my_procs, int my_root_rank, int  my_num_elements) {
    MPI_Status my_status;

    if (my_rank == my_root_rank) {
        for (int my_i = 1; my_i < my_procs; my_i++){
            MPI_Send(my_data,  my_num_elements , MPI_INT, my_i,  0, MPI_COMM_WORLD);
        }
    }
    else {
        MPI_Recv(my_data,   my_num_elements , MPI_INT, my_root_rank,  0, MPI_COMM_WORLD, &my_status);
    }
}

