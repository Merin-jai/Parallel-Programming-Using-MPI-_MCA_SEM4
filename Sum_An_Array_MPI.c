#include <mpi.h>
#include <stdio.h>

#define ARRAY_SIZE 8  // Size of the array

int main(int argc, char *argv[]) {
    int rank, size;
    int arr[ARRAY_SIZE] = {1, 2, 3, 4, 5, 6, 7, 8};  // Input array
    int local_sum = 0, global_sum = 0;
    int local_size, start, end;

    MPI_Init(&argc, &argv);  // Initialize MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Divide the array among processes
    local_size = ARRAY_SIZE / size;
    start = rank * local_size;
    end = start + local_size;

    // Compute partial sum
    for (int i = start; i < end; i++) {
        local_sum += arr[i];
    }

    printf("Process %d computed local sum = %d\n", rank, local_sum);

    // Reduce all partial sums to global sum at rank 0
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // Root process prints final sum
    if (rank == 0) {
        printf("Total sum = %d\n", global_sum);
    }

    MPI_Finalize();  // Finalize MPI
    return 0;
}
