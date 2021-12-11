CC = gcc
CFLAGS = -I. -g -pthread -w
DEPS = randomgen.h 
OBJ = randomgen.o wordpuzzle.o

%.o: %.c 
	$(CC) -c -o $@ $< $(CFLAGS)
