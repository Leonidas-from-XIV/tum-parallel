all: shift

shift: shift.o
	mpicc -O3 -o shift shift.o

shift.o: shift.c
	mpicc -O3 -c shift.c

