#include <stdio.h>
#include <string.h>
#include <mpi.h>

int main(int argc, char** argv)
{
    int rank, size, normalized;
    char buf[255];
    MPI_File f;
    MPI_Status status;

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS) {
        printf("Error in init\n");
        return 1;
    }
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    normalized = rank + 1;

    // format the message to write
    sprintf(buf, "Hello from process %d of %d\n", normalized, size);

    MPI_File_open(MPI_COMM_WORLD, "rank1.file", MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &f);
    MPI_File_write_ordered(f, buf, strlen(buf), MPI_CHAR, &status);

    MPI_File_close(&f);
    MPI_Finalize();
    return 0;
}
