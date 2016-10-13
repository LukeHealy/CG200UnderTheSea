CC = gcc
CFLAGS = -Wall -pedantic -g -std=c99 -Wno-deprecated-declarations
OPENGL = -framework GLUT -framework OpenGL
OBJ = keyActions.o underthe.o bmploader.o
EXEC = underthe

all: $(OBJ) $(EXEC)

keyActions.o: keyActions.c
	$(CC) $(CFLAGS) -c keyActions.c keyActions.h

underthe.o: underthe.c
	$(CC) $(CFLAGS) -c underthe.c keyActions.h

bmpreader.o: bmpreader.c
	$(CC) $(CFLAGS) -c bmpreader.c bmploader.h

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OPENGL) $(OBJ) -o $(EXEC)

clean:
	rm -f $(EXEC); rm -f $(OBJ); clear; clear