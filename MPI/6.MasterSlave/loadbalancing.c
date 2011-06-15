#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N_R      79
#define N_I      100
#define V_R_MIN -1.0
#define V_R_MAX  1.0
#define V_I_MIN -1.0
#define V_I_MAX  1.0
static int icalman(float, float);

void main()
{
  int     field[N_R][N_I];
  char    fieldc[N_R][N_I];
  float   v_r, v_i;
  float   v_r_step, v_i_step;
  char    schedenv[13] = "OMP_SCHEDULE";
  char    *sched;
  int     i,j;

  sched = getenv( schedenv );
  v_r_step = (V_R_MAX-V_R_MIN)/N_R;
  v_i_step = (V_I_MAX-V_I_MIN)/N_I;

  for( i=1; i<=N_R; i++ ) {
      v_r = V_R_MIN+(i-1)*v_r_step;
      for( j=1; j<=N_I; j++) {
        v_i = V_I_MIN+(j-1)*v_i_step;
        field[i-1][j-1] = icalman(v_r, v_i);
      }
    }

  for( i=1; i<=N_R; i++ ) {
      for( j=1; j<=N_I; j++) {
        if( field[i-1][j-1] == -1 )
          fieldc[i-1][j-1] = '*';
        else
          fieldc[i-1][j-1] = ' ';
      }
    }

  for( j=1; j<=N_I; j++ ) {
    for( i=1; i<=N_R; i++ ) {
      printf( "%c", fieldc[i-1][j-1] );
    }
    printf( "\n" );
  }
  printf( "%s=%s\n", schedenv, sched);
}

#define CMAX 10000
static int icalman(float vr, float vi)
{
  struct { float r,i; } cc, zz, tmp;
  int i;

  cc.r = vr;
  cc.i = vi;
  zz.r = 0.0;
  zz.i = 0.0;
  for( i=1; i<=CMAX; i++ ) {
    tmp.r = (zz.r*zz.r-zz.i*zz.i)+cc.r;
    tmp.i = (zz.r*zz.i+zz.i*zz.r)+cc.i;
    zz.r = tmp.r;
    zz.i = tmp.i;
    if((zz.r*zz.r)+(zz.i*zz.i) > 4.0 )
      return i;
  }
  return -1;
}
