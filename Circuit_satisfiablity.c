#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

// Example Boolean Circuit: (x1 OR x2) AND (NOT x3)
int circuit(int bits) {
    int x1 = (bits >> 0) & 1;
    int x2 = (bits >> 1) & 1;
    int x3 = (bits >> 2) & 1;
    return (x1 || x2) && (!x3);
}

int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int num_inputs = 8; // 3-bit inputs (0-7)
    int found = 0;  // Flag to indicate if a solution is found
    int local_found = 0;

    // Divide the work among processes
    for (int i = rank; i < num_inputs; i += size) {
        if (circuit(i)) {
            local_found = 1;
            printf("Process %d found a satisfying input: %d\n", rank, i);
            break;
        }
    }

    // Reduce to check if any process found a solution
    MPI_Reduce(&local_found, &found, 1, MPI_INT, MPI_LOR, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        if (found)
            printf("Circuit is satisfiable!\n");
        else
            printf("Circuit is NOT satisfiable.\n");
    }

    MPI_Finalize();
    return 0;
}
