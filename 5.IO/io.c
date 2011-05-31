#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv)
{
    int rank, size;
    if (MPI_Init(&argc, &argv) != MPI_SUCCESS) {
        printf("Error in init\n");
        return 1;
    }
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    printf("Hello from process %d of %d\n",
            rank+1, size);

    MPI_Finalize();
    return 0;
}
