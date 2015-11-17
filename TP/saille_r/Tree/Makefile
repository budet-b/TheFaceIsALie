CC=gcc
CFLAGS= -Wall -Wextra -Werror -std=c99 -O2
SRC= tree.c testing.c
OBJ= $(SRC:.c=.o)

all: $(OBJ)
	$(CC) $(CFLAGS) -o tree $(OBJ)

clean:
	$(RM) $(OBJ) tree testing
