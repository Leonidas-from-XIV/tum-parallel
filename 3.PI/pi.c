#include <stdio.h>

#define f(A) (4.0/(1.0+A*A))
/* Serial Program
   Approximate pi with the n-point rectangle quadrature rule
   applied to the integral from 0 to 1 of 4 / (1+x**2).   */

const int n = 10000000;

int main(int argc, char** argv)
{
  int i;
  double w,x,sum,pi;

/* calculate the interval size */
  w=1.0/n;
  sum=0.0;
  for (i=1;i<=n;i++)
  {
    x=w*((double)i-0.5);
    sum=sum+f(x);
  }
  pi=w*sum;

  printf( "computed pi = %24.16g\n", pi );
  return 0;
}
