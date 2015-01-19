###############################################################################
# CS/CNS 171 Fall 2014
#
# This is a template Makefile for the assignment. Edit it however you find
# convenient, though the only line that you will probably need to change for
# Assignment 0 is the INCLUDE line. For Assignment 1, you will need to change
# the EXENAME.
###############################################################################
CC = g++
FLAGS = -g -o
# Change the following directory to whatever directory has your Eigen folder.
INCLUDE = -I../lib
SOURCES = *.h util.cpp parser_test.cpp openivparser.tab.cpp lex.openiv.c

# Please change the EXENAME to "wireframe" for Assignment 1.
EXENAME = parser_test

all: $(SOURCES)
	$(CC) $(FLAGS) $(EXENAME) $(INCLUDE) $(SOURCES)

clean:
	rm -f *.o $(EXENAME) lex.openiv.c openivparser.tab.cpp \
	openivparser.tab.hpp

openivparser.tab.cpp openivparser.tab.hpp: openivparser.ypp
	bison -d openivparser.ypp

lex.openiv.c: openivlexer.lex
	flex openivlexer.lex

.PHONY: all clean

