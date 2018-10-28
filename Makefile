CC = gcc
CFLAGS = -Wall -I. -g -O0
DEPS = file_ops.h render.h read_keypress.h init.h error_handler.h insert.h cursor_movement.h
OBJ = file_ops.o render.o read_keypress.o init.o error_handler.o insert.o cursor_movement.o
%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

svim: $(OBJ)
	$(CC) $(CFLAGS) main.c -o $@ $^

.PHONY: clean

clean:
	rm *.o
