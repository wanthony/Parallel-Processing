CC=gcc
LIBS=-lm
CFLAGS=-g $(LIBS)

fourier: main.c complex.o
	$(CC) -ofourier main.c complex.o $(CFLAGS)

complex.o: complex.c complex.h
	$(CC) -ocomplex.o -c complex.c $(CFLAGS)

clean:
	rm -rf *.o fourier
