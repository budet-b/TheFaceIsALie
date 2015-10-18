# Makefile
 
# Compilers and options
 
# Setup compiler
# Add runtime aggressive memory check
CC=clang -fsanitize=address
# Prefer gcc without sanitizer for production
# CC=gcc
 
# CPPFLAGS with dependencies files (.d)
CPPFLAGS= -MMD -MP
 
# Standard options, added pedantic
CFLAGS= -Wall -Wextra -pedantic -std=c99 -O2
 
# libs options
LDFLAGS=
LDLIBS=
 
# source files
SRC= arrays.c ptr_arrays.c
# object files (produced from source files list)
OBJ= ${SRC:.c=.o}
 
# dependencies files (produced from source files list)
DEP= ${SRC:.c=.d}
 
# include dependencies files
-include ${DEP}
 
# default rule
all: ptr_arrays
 
# ptr_arrays depends on object files
ptr_arrays: ${OBJ}
 
# setup cleaning rules as phony: always run
.PHONY: clean
 
# Cleaning rule
clean:
	rm -f *.o *~
	rm -f ${DEP}
	rm -f ptr_arrays
 
 # END
