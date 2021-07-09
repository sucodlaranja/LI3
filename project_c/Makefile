CC = gcc
SOURCES=$(wildcard src/*.c)

CFLAGS = `pkg-config --cflags --libs glib-2.0` -std=c99 -Wall
OPTIFLAGS = -O2

OBJ=$(patsubst %.c, %.o, $(SOURCES))



program: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(OPTIFLAGS)


clean:
	-rm  $(wildcard src/*.o) program