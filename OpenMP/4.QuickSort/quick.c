#include <stdio.h>

/*                        */
/*  Quick sort code in C  */
/*                        */

qsort( a, lo, hi ) int a[], hi, lo;
{
  int h, l, p, t;
 
  if (lo < hi) {
    l = lo;
    h = hi;
    p = a[hi];
 
    do {
      while ((l < h) && (a[l] <= p))
          l = l+1;
      while ((h > l) && (a[h] >= p))
          h = h-1;
      if (l < h) {
          t = a[l];
          a[l] = a[h];
          a[h] = t;
      }
    } while (l < h);
 
    t = a[l];
    a[l] = a[hi];
    a[hi] = t;
    qsort( a, lo, l-1 );
    qsort( a, l+1, hi );
  }
}

main()
{
  int Array[4];
  int i;

  Array[0]=3;
  Array[1]=2;
  Array[2]=4;
  Array[3]=1;
   
  printf("before sort: ");
  for (i=0;i<4;i++)
     printf("%d",Array[i]);
  qsort(Array,0,3);
  printf("\n after sort: ");
  for (i=0;i<4;i++)
     printf("%d",Array[i]);
  printf("\n");
}
