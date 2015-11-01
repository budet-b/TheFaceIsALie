CC=clang
CPPFLAGS= `pkg-config --cflags sdl --libs gtk+-2.0`
CFLAGS=-Wall -Wextra -pedantic -std=c99 -g3 -ggdb
LDFLAGS=
LDLIBS= `pkg-config --libs sdl` -lSDL_image

SRC= image.c haar.c main.c
OBJ= ${SRC:.c=.o}

main: ${OBJ}

clean:
	rm -f main
	rm -f *~ *.o
	rm -f ToGray.bmp

