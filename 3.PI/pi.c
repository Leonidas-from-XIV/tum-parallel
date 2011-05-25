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

    // calculate the interval size
    // old
    /*
    w = 1.0 / n;
    sum = 0.0;
    for (i = 1; i <= n; i++) {
	x = w * ((double) i - 0.5);
	sum = sum + f(x);
    }
    pi = w * sum;
    */

    // new
    w =  1.0 / n;
    sum = 0.0;
    double sbuf[np];
    pi = 3;
    double dummy = 0.0;

    chunksize = n / rank;

    switch (rank) {
    case 0:
        //MPI_Reduce(sbuf, &sum, np, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
        MPI_Gather(&dummy, np, MPI_DOUBLE, sbuf, np, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        for (i = 0; i <= np; i++) {
            sum = sum + sbuf[i];
        }
        printf("computed pi = %24.16g\n", sum);
        break;
    default:

        for (i = 1; i <= n; i++) {
            x = w * ((double) i - 0.5);
            sum = sum + f(x);
        }
        MPI_Gather(&sum, 1, MPI_DOUBLE, sbuf, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        break;
    }


    MPI_Finalize();
    return 0;
}
