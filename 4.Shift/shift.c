#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

main (int argc,char *argv[]){
int myid, np, ierr, lnbr, rnbr, shifts, i, j;
int *values;
MPI_Status status;

ierr = MPI_Init (&argc, &argv);
if (ierr != MPI_SUCCESS){
  printf("Cannot initialize MPI!\n");
  MPI_Finalize();
  exit(0);
}

MPI_Comm_size(MPI_COMM_WORLD, &np);
MPI_Comm_rank(MPI_COMM_WORLD, &myid);
if (100%np!=0){
  if (myid==0){
    printf("Error: cannot execute with %d processors!\n", np);
  }
  MPI_Finalize();
  exit(0);
}

if (myid==0){
  lnbr=np-1;
  rnbr=myid+1;
}
else if (myid==np-1){
  lnbr=myid-1;
  rnbr=0;
}
else{
  lnbr=myid-1;
  rnbr=myid+1;
}


shifts=atoi(argv[1]);
values= (int *) calloc(100/np,sizeof(int));

if (myid==0){
  values[0]=1;
}


for (i=0;i<shifts;i++){
  if (myid==0){
    MPI_Send(&values[0], 1, MPI_INT, lnbr, 10, MPI_COMM_WORLD);
    for (j=1;j<100/np;j++){
      values[j-1]=values[j];
    }
    MPI_Recv(&values[100/np-1], 1, MPI_INT, rnbr, 10, MPI_COMM_WORLD, &status);
  }
  else{
    int buf=values[0];
    for (j=1;j<100/np;j++){
      values[j-1]=values[j];
    }
    MPI_Recv(&values[100/np-1], 1, MPI_INT, rnbr, 10, MPI_COMM_WORLD, &status);
    MPI_Send(&buf, 1, MPI_INT, lnbr, 10, MPI_COMM_WORLD);
  }
}


if (myid==0){
  int flag=1;
  for (i=0;i<100/np;i++)
    printf(" %2d:%4d\n", i, values[i]);
  MPI_Send(&flag, 1, MPI_INT, rnbr, 20, MPI_COMM_WORLD);
}
else{
  int flag;
  MPI_Recv(&flag, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
  for (i=0;i<100/np;i++)
    printf(" %2d:%4d\n", i+myid*(100/np), values[i]);
  if (myid!=np-1)
    MPI_Send(&flag, 1, MPI_INT, rnbr, 20, MPI_COMM_WORLD);
}
MPI_Finalize();
}
