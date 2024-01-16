CC=gcc
CFLAGS=-I. -w -lm -pthread
DEPS = hash.h
OBJ = passwordgen.o hashset.o
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
passgenerator: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)