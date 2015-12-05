CC=clang
CPPFLAGS= `pkg-config --cflags sdl --libs gtk+-2.0`
CFLAGS=-Wall -Wextra -pedantic -std=c99 -g3 -ggdb
LDFLAGS=
LDLIBS= `pkg-config --libs sdl` -lSDL_image -lm

SRC= main.c image.c haar.c adaboost.c adabase.c process.c
OBJ= ${SRC:.c=.o}

main: ${OBJ}

clean:
	rm -f main
	rm -f *~ *.o
	rm -f ToGray.bmp

