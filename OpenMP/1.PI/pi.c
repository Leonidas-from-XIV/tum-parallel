#include <stdio.h>
#include <omp.h>

#define f(A) (4.0/(1.0+A*A))
/* Serial Program
   Approximate pi with the n-point rectangle quadrature rule
   applied to the integral from 0 to 1 of 4 / (1+x**2).   */

const long n = 100000000;

int main(int argc, char **argv)
{
    long i;
    double w, sum, pi;
    double ts, te;
    volatile double x;

    /* calculate the interval size */
    w = (double) 1.0 / (double) n;
    sum = 0.0;
    ts = omp_get_wtime();

#pragma omp parallel for private(x)
    for (i = 1; i <= n; i++) {
	x = w * ((double) i - 0.5);
#pragma omp critical
        {
	sum = sum + f(x);
        }
    }

    pi = w * sum;
    te = omp_get_wtime();

    printf("computed pi = %24.16g  in  %5.2f seconds (%5.2f GFlops)\n", pi,
	   te - ts, 3 * n / (te - ts) / 1E9);

    return 0;
}
