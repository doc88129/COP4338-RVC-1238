CC=gcc
CFLAGS=-I. -g -w
DEPS = 
OBJ = convert.o 
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
convert: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)