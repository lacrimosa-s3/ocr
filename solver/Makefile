# Makefile

CC = gcc
CFLAGS = `pkg-config --cflags gtk+-3.0` -c
LDLIBS = `pkg-config --libs gtk+-3.0`

EXE = interface

all: $(EXE)

$(foreach f, $(EXE), $(eval $(f):))

.PHONY: clean

clean:
		${RM} $(EXE)

# END

