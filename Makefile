CC=gcc
INCLUDE=-I.
DFLAGS=-DTEST
LIBS=
OFLAGS=-c
CFLAGS=-g3 -Wall -Wextra -std=c99 -fno-builtin -pedantic-errors $(INCLUDE) $(DFLAGS)

OBJECTS=vsscanf.o test_vsscanf.o
TARGET=test

all: $(OBJECTS) Makefile
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)

%.o: %.c Makefile *.h
	$(CC) $(CFLAGS) $(OFLAGS) -o $@ $<
run:
	./$(TARGET)
clean:
	rm $(OBJECTS) $(TARGET)
