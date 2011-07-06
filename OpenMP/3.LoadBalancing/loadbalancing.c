#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define N_R      79
#define N_I      100
#define V_R_MIN -1.0
#define V_R_MAX  1.0
#define V_I_MIN -1.0
#define V_I_MAX  1.0
static int icalman(float, float);

void main()
{
    int field[N_R][N_I];
    char fieldc[N_R][N_I];
    float v_r, v_i;
    float v_r_step, v_i_step;
    char schedenv[13] = "OMP_SCHEDULE";
    char *sched;
    int i, j;
    double* time_per_thread;
    int num_threads;

    sched = getenv(schedenv);
    v_r_step = (V_R_MAX - V_R_MIN) / N_R;
    v_i_step = (V_I_MAX - V_I_MIN) / N_I;

    char *num_threads_env = getenv("OMP_NUM_THREADS");
    if (!num_threads_env) {
        num_threads = 1;
    }
    else {
        num_threads = atoi(num_threads_env);
    }

    time_per_thread = calloc(num_threads, sizeof(double));

    for (i = 1; i <= N_R; i++) {
	v_r = V_R_MIN + (i - 1) * v_r_step;
#pragma omp parallel for
	for (j = 1; j <= N_I; j++) {
	    v_i = V_I_MIN + (j - 1) * v_i_step;
            double start = omp_get_wtime();
	    field[i - 1][j - 1] = icalman(v_r, v_i);
            double end = omp_get_wtime();
            int current_thread = omp_get_thread_num();
            time_per_thread[current_thread] += end - start;
	}
    }

    for (i = 1; i <= N_R; i++) {
	for (j = 1; j <= N_I; j++) {
	    if (field[i - 1][j - 1] == -1)
		fieldc[i - 1][j - 1] = '*';
	    else
		fieldc[i - 1][j - 1] = ' ';
	}
    }

    for (j = 1; j <= N_I; j++) {
	for (i = 1; i <= N_R; i++) {
	    printf("%c", fieldc[i - 1][j - 1]);
	}
	printf("\n");
    }
    printf("%s=%s\n", schedenv, sched);

    for (i = 0; i < num_threads; i++) {
        printf("Execution time of thread %d: %f\n", i+1, time_per_thread[i]);
    }
}

#define CMAX 10000
static int icalman(float vr, float vi)
{
    struct {
	float r, i;
    } cc, zz, tmp;
    int i;

    cc.r = vr;
    cc.i = vi;
    zz.r = 0.0;
    zz.i = 0.0;
    for (i = 1; i <= CMAX; i++) {
	tmp.r = (zz.r * zz.r - zz.i * zz.i) + cc.r;
	tmp.i = (zz.r * zz.i + zz.i * zz.r) + cc.i;
	zz.r = tmp.r;
	zz.i = tmp.i;
	if ((zz.r * zz.r) + (zz.i * zz.i) > 4.0)
	    return i;
    }
    return -1;
}
