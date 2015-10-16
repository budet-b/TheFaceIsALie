CC=clang
CPPFLAGS= `pkg-config --cflags sdl --libs gtk+-2.0`
CFLAGS=-Wall -Wextra -pedantic -std=c99 -g3 -ggdb
LDFLAGS=
LDLIBS= `pkg-config --libs sdl` -lSDL_image

SRC= main.c
OBJ= ${SRC:.c=.o}

all:RF

RF:RF.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDLIBS) RF.c -o rf

Interface:Interface.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDLIBS) Interface.c -o inter

test:test.c
	$(CC) $(CFLAGS) test.c -o test	

Database:DB.c
	$(CC) $(CFLAGS) DB.c -o db

main: ${OBJ}

clean:
	rm -f main
	rm -f *~ *.o
