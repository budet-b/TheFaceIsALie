CC = gcc
CPPFLAGS = `pkg-config --cflags sdl --libs gtk+-2.0`
CPFLAGS = -Wall -Wextra -pedantic -std=c99 -g3 -ggdb -ainsi -pedantic
LDLIBS= `pkg-config --libs sdl` -lSDL_image -lm
SRC = main.c error.c callback.c image.c haar.c adaboost.c adabase.c process.c
OBJ= ${SRC: .c=.o}
main: ${OBJ}
clean: 
	rm -f *~ *.o
	rm -f main
