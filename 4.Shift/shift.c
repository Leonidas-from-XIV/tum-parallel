#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

main(int argc, char *argv[])
{
    int myid, np, ierr, lnbr, rnbr, shifts, i, j;
    int *values;
    MPI_Status status;

    ierr = MPI_Init(&argc, &argv);
    if (ierr != MPI_SUCCESS) {
	printf("Cannot initialize MPI!\n");
	MPI_Finalize();
	exit(0);
    }

    MPI_Comm_size(MPI_COMM_WORLD, &np);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    if (100 % np != 0) {
	if (myid == 0) {
	    printf("Error: cannot execute with %d processors!\n", np);
	}
	MPI_Finalize();
	exit(0);
    }

    if (myid == 0) {
	lnbr = np - 1;
	rnbr = myid + 1;
    } else if (myid == np - 1) {
	lnbr = myid - 1;
	rnbr = 0;
    } else {
	lnbr = myid - 1;
	rnbr = myid + 1;
    }


    shifts = atoi(argv[1]);
    values = (int *) calloc(100 / np, sizeof(int));

    if (myid == 0) {
	values[0] = 1;
    }

    MPI_Request req_in, req_out;
    int inbuf;
    int outbuf;

    for (i = 0; i < shifts; i++) {
        // save previous value for sending
        outbuf = values[0];

        // send and receive in the background
        MPI_Isend(&outbuf, 1, MPI_INT, lnbr, 10, MPI_COMM_WORLD, &req_out);
        MPI_Irecv(&inbuf, 1, MPI_INT, rnbr, 10,
                 MPI_COMM_WORLD, &req_in);

        // local computation
        for (j = 1; j < 100 / np; j++) {
            values[j - 1] = values[j];
        }

        // wait for delivery of inbuf so we can set it
        MPI_Wait(&req_in, &status);
        values[100 / np - 1] = inbuf;
    }

    int *reg;
    int root = 0;
    int chunksize = 100 / np;

    if (myid == root) {
        /* only initialize if root process */
        reg = (int *) calloc(100, sizeof (int));
    }
    MPI_Gather(values, chunksize, MPI_INT, reg, chunksize, MPI_INT, root, MPI_COMM_WORLD);

    if (myid == 0) {
	for (i = 0; i < 100; i++)
	    printf(" %2d:%4d\n", i, reg[i]);
    }

    MPI_Finalize();
}
