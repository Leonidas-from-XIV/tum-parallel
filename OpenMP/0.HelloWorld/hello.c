#include <stdio.h>
#include <omp.h>

int main(int argc, char **argv)
{
#pragma omp parallel
    {
    printf("Hello World from %d of %d.\n",
            omp_get_thread_num() + 1,
            omp_get_num_threads());
    }

    return 0;
}
