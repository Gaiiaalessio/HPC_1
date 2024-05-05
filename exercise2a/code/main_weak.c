#include "bcast_binary_tree.c"
#include "bcast_chain.c"
#include "bcast_flat.c"
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>

#define WARMUP_ITERATIONS 1000
#define NUM_RUNS 1000

double calculate_average(double *array, int length);

int main(int argc, char *argv[]) {
    int my_rank, my_procs;
    int my_root_rank = 0;
    double my_start_time_bintree, my_end_time_bintree;
    double my_start_time_chain, my_end_time_chain;
    double my_start_time_flat, my_end_time_flat;
    double my_start_time_bcast, my_end_time_bcast;
    int *my_data = NULL;
    char my_filename[] = "weak_scaling_final.csv";
    long long int my_size = 1024; // Fixed data size to 1024
    MPI_Init(&argc, &argv);
    int my_warmup_value = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &my_procs);

    int my_dummy_data = 0;
    // Warmup communication
    for (int my_i = 0; my_i < WARMUP_ITERATIONS; ++my_i) {
        my_binary_tree_broadcast(&my_dummy_data, my_rank, my_procs, my_root_rank, 1);
        my_chain_broadcast(&my_dummy_data, my_rank, my_procs, my_root_rank, 1);
        my_flat_tree_broadcast(&my_dummy_data, my_rank, my_procs, my_root_rank, 1);
    }

    double my_results_bintree[NUM_RUNS];
    double my_results_chain[NUM_RUNS];
    double my_results_flat[NUM_RUNS];
    double my_results_bcast[NUM_RUNS];

    // Execute NUM_RUNS times for each algorithm
    for (int my_i = 0; my_i < NUM_RUNS; my_i++) {
        // Allocate data
        my_data = (int *)malloc(my_size * sizeof(int));
        if (my_data == NULL) {
            fprintf(stderr, "Error allocating memory for data\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        // Initialize data on root process
        if (my_rank == 0) {
            for (int my_j = 0; my_j < my_size; my_j++) {
                my_data[my_j] = my_j;
            }
        }

        // Barrier to synchronize processes
        MPI_Barrier(MPI_COMM_WORLD);
        my_start_time_bintree = MPI_Wtime(); // Start MPI timer
        my_binary_tree_broadcast(my_data, my_rank, my_procs, my_root_rank, my_size);
        my_end_time_bintree = MPI_Wtime(); // Stop MPI timer
        my_results_bintree[my_i] = my_end_time_bintree - my_start_time_bintree;

        // Barrier to synchronize processes
        MPI_Barrier(MPI_COMM_WORLD);
        my_start_time_chain = MPI_Wtime(); // Start MPI timer
        my_chain_broadcast(my_data, my_rank, my_procs, my_root_rank, my_size);
        my_end_time_chain = MPI_Wtime(); // Stop MPI timer
        my_results_chain[my_i] = my_end_time_chain - my_start_time_chain;

        // Barrier to synchronize processes
        MPI_Barrier(MPI_COMM_WORLD);
        my_start_time_flat = MPI_Wtime(); // Start MPI timer
        my_flat_tree_broadcast(my_data, my_rank, my_procs, my_root_rank, my_size);
        my_end_time_flat = MPI_Wtime(); // Stop MPI timer
        my_results_flat[my_i] = my_end_time_flat - my_start_time_flat;

        MPI_Barrier(MPI_COMM_WORLD);
        my_start_time_bcast = MPI_Wtime();
        MPI_Bcast(my_data, my_size, MPI_INT, my_root_rank, MPI_COMM_WORLD);
        my_end_time_bcast = MPI_Wtime();
        my_results_bcast[my_i] = my_end_time_bcast - my_start_time_bcast;

        // Free allocated memory
        free(my_data);
    }

    // Calculate average time
    double my_avg_time_bintree = calculate_average(my_results_bintree, NUM_RUNS);
    double my_avg_time_chain = calculate_average(my_results_chain, NUM_RUNS);
    double my_avg_time_flat = calculate_average(my_results_flat, NUM_RUNS);
    double my_avg_time_bcast = calculate_average(my_results_bcast, NUM_RUNS);

    double my_max_time_bintree;
    double my_max_time_chain;
    double my_max_time_flat;
    double my_max_time_bcast;
    MPI_Reduce(&my_avg_time_bintree, &my_max_time_bintree, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&my_avg_time_chain, &my_max_time_chain, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&my_avg_time_flat, &my_max_time_flat, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&my_avg_time_bcast, &my_max_time_bcast, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    // Only process with rank 0 writes results to the file
    if (my_rank == 0) {
        FILE *fp = fopen(my_filename, "a");
        if (fp != NULL) {
            fprintf(fp, "processes,size,bintree,chain,flat,bcast\n");
            fprintf(fp, "%d,%lld,%f,%f,%f,%f\n", my_procs, my_size, my_max_time_bintree * 1000000, my_max_time_chain * 1000000, my_max_time_flat * 1000000, my_max_time_bcast * 1000000);
            fclose(fp);
        } else {
            printf("Error: Unable to open file %s\n", my_filename);
        }
    }

    MPI_Finalize();
    return 0;
}

double calculate_average(double *array, int length) {
    double sum = 0;
    for (int i = 0; i < length; i++) {
        sum += array[i];
    }
    return sum / length;
}

