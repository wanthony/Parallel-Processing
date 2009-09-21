CC=gcc
CFLAGS=-g $(LIBS) -L"/usr/lib/mpich/lib" -I"/usr/lib/mpich/include"
LIBS=-lmpi

trap: main.c
	$(CC) -otrap main.c $(CFLAGS)

clean:
	rm -rf trap