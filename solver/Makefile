# Makefile

CPPFLAGS = -MMD
CC = gcc
CFALGS = -Wall -Wextra
LDFLAGS =
LDLIBS =

SRC = main.c solver.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

solver: ${OBJ}

-include ${DEP}

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${DEP}
	${RM} solver

# END
