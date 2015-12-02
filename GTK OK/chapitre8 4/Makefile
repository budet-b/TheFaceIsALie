CC = gcc
CPPFLAGS = `pkg-config --cflags --libs gtk+-2.0`
CPFLAGS = -Wall -Wextra -pedantic -std=c99 -W -O2 -ainsi -pedantic
LDLIBS= 
SRC = main.c error.c callback.c
OBJ= ${SRC: .c=.o}
main: ${OBJ}
clean: 
	rm -f main
