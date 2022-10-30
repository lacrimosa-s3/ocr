# Simple SDL mini Makefile

CC=gcc

CPPFLAGS= -MMD
CFLAGS= `pkg-config --cflags sdl` `pkg-config --cflags glib-2.0` -Wall -Wextra -std=c99 -g 
LDFLAGS= 
LDLIBS= `pkg-config --libs sdl` `pkg-config --libs glib-2.0` -lSDL_image -lm

all: hough_lines

hough_lines: display.o image_modification.o hough_lines.o


clean:
	${RM} *.o
	${RM} *.d
	${RM} display
	${RM} hough_lines

# END
