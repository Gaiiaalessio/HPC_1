#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void my_binary_tree_broadcast(int *my_data, int my_rank, int my_procs, int my_root_rank, int my_num_elements) {
    MPI_Status my_status;
    int my_parent_rank = (my_rank -  1) /  2;
    int my_left_child_rank =  2 * my_rank +  1;
    int my_right_child_rank =  2 * my_rank +  2;

    if (my_rank == my_root_rank) {
        if (my_left_child_rank < my_procs)
            MPI_Send(my_data,  my_num_elements, MPI_INT, my_left_child_rank,  0, MPI_COMM_WORLD);
        if (my_right_child_rank < my_procs)
            MPI_Send(my_data,  my_num_elements, MPI_INT, my_right_child_rank,  0, MPI_COMM_WORLD);
    } else {
        MPI_Recv(my_data, my_num_elements, MPI_INT, my_parent_rank,  0, MPI_COMM_WORLD, &my_status);
        if (my_left_child_rank < my_procs)
            MPI_Send(my_data, my_num_elements, MPI_INT, my_left_child_rank,  0, MPI_COMM_WORLD);
        if (my_right_child_rank < my_procs)
            MPI_Send(my_data, my_num_elements, MPI_INT, my_right_child_rank,  0, MPI_COMM_WORLD);
    }
}

