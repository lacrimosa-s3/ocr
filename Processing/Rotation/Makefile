# Simple SDL mini Makefile

CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra -Werror -std=c99 -O3 
LDFLAGS= 
LDLIBS= `pkg-config --libs sdl` -lSDL_image -lm
SDLLIB=`sdl2-config --cflags --libs`

all: rotation.o SDL_tools.o
	${CC} ${CPPFLAGS} ${LDLIBS} -o main *.o

clean:
	${RM} *.o
	${RM} *.d
	${RM} main

# END
