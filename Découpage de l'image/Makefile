# Simple SDL mini Makefile

CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra -Werror -std=c99 -g
LDFLAGS=
LDLIBS= `pkg-config --libs sdl` -lSDL_image -lm

all: hough_lines

hough_lines: display.o image_modification.o hough_lines.o


clean:
	${RM} *.o
	${RM} *.d
	${RM} display
	${RM} hough_lines

# END
