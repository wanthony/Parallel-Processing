CC=gcc
LIBS=-lm
CFLAGS=-g $(LIBS)

ftrans: main.c complex.o
	$(CC) -oftrans main.c complex.o $(CFLAGS)

complex.o: complex.c complex.h
	$(CC) -ocomplex.o -c complex.c $(CFLAGS)

clean:
	rm -rf *.o ftrans
