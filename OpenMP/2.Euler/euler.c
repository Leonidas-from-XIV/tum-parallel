#include <stdio.h>
#define M 4000000

#undef M
#define M 400000

int main()
{
    double uc[M + 1], un[M + 1];
    double r, r1, dx, dt;
    int timcnt;
    int i, j;

    dx = 0.1e-2;
    dt = 0.1e-6;
    timcnt = 250;

    r = dt / (dx * dx);
    r1 = 1 - 2 * r;

#pragma omp parallel for
    for (i = 0; i < M; i++) {
	uc[i] = 1.0;
    }

    uc[0] = 0.0;
    uc[M] = 0.0;

    /* work happens here */
    for (j = 0; j < timcnt; j++) {
        {
#pragma omp for
            for (i = 1; i < M; i++) {
                un[i] = r * (uc[i - 1] + uc[i + 1]) + r1 * uc[i];
            }
#pragma omp for
            for (i = 1; i < M; i++) {
                uc[i] = un[i];
            }
        }
    }

    double sum = 0.0;
    for (i = 1; i < M - 1; i++) {
	sum = sum + uc[i];
    }
    printf("sum  %9.8f\n", sum);

    return 0;
}
