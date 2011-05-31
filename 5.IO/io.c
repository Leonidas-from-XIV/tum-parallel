#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv)
{
    int rank, size, normalized;
    FILE* f;
    char filename[255];

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS) {
        printf("Error in init\n");
        return 1;
    }
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    normalized = rank + 1;

    sprintf(filename, "rank%d.file", normalized);

    f = fopen(filename, "w");
    fprintf(f, "Hello from process %d of %d\n",
            normalized, size);

    fclose(f);
    MPI_Finalize();
    return 0;
}
