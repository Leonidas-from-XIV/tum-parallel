#include <stdio.h>
#include <mpi.h>

#define f(A) (4.0/(1.0+A*A))
/* Serial Program
   Approximate pi with the n-point rectangle quadrature rule
   applied to the integral from 0 to 1 of 4 / (1+x**2).   */

const int n = 10000000;

int main(int argc, char **argv)
{
    int i, np, rank;
    double w, x, sum, pi;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &np);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    w =  1.0 / n;
    sum = 0.0;
    double rbuf[np];
    double acumulator;
    int root = 0;

    int chunksize = n / np;
    int lower_bound = rank * chunksize + 1;
    int upper_bound = lower_bound + chunksize - 1;

    for (i = lower_bound; i <= upper_bound; i++) {
        x = w * ((double) i - 0.5);
        sum = sum + f(x);
    }

    MPI_Gather(&sum, 1, MPI_DOUBLE, rbuf, 1, MPI_DOUBLE, root, MPI_COMM_WORLD);

    if (rank == 0) {
        for (i = 0; i < np; i++) {
            /* ** optimizer bug in GCC **
             * insert this shit and compile with -O3 or you need to add
             * -fno-caller-save (enabled by -O2) to the code so it compiles
             */
            /*printf("\n"); */
            /* end of hack */
            acumulator += rbuf[i];
        }
        pi = w * acumulator;
        printf("computed pi = %24.16g\n", pi);
    }

    MPI_Finalize();
    return 0;
}
