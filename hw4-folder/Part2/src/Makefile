###############################################################################
# CS/CNS 171 Fall 2014
#
# This is a template Makefile for Assignment 3. It is meant to compile your
# OpenGL program while also taking into account the Flex and Bison dependence.
# Edit it however you find convenient.
# 
# The current version of this file should compile your program just fine on
# Debian-based Linux operating systems.
#
# If you run Mac OS or other distributions of Linux, then you may have to
# fool around with the 'INCLUDE' and 'LIBDIR' lines below before the Makefile
# can compile the OpenGL parts successfully.
###############################################################################
CC = g++
FLAGS = -g

INCLUDE = -I/usr/X11R6/include -I/usr/include/GL -I/usr/include -I../lib
LIBDIR = -L/usr/X11R6/lib -L/usr/local/lib
SOURCES = keyframe.cpp openivparser.tab.cpp lex.openiv.c util.o
LIBS = -lGLEW -lGL -lGLU -lglut -lm

EXENAME = keyframe

all: $(SOURCES)
	$(CC) $(FLAGS) $(INCLUDE) $(LIBDIR) $(SOURCES) $(LIBS) -o $(EXENAME)

util.o: util.cpp
	$(CC) $(FLAGS) $(LIBDIR) -c util.cpp

clean:
	rm -f *.o $(EXENAME) lex.openiv.c openivparser.tab.cpp \
	openivparser.tab.hpp

openivparser.tab.cpp openivparser.tab.hpp: openivparser.ypp
	bison -d openivparser.ypp

lex.openiv.c: openivlexer.lex
	flex openivlexer.lex

.PHONY: all clean
