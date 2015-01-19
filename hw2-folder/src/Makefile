###############################################################################
# CS 171 Assignment by JongSeok (Parker) Won
#
# Makefile for shaded
# The original Makefile template was altered to improve recompilation time
###############################################################################
CC = g++
FLAGS = -g
INCLUDE = -I../lib
SOURCES = shaded.cpp openivparser.tab.cpp lex.openiv.c util.o ivdata_util.o
EXENAME = shaded

all: $(SOURCES)
	$(CC) $(FLAGS) $(INCLUDE) $(SOURCES) -o $(EXENAME)

util.o: util.cpp
	$(CC) $(FLAGS) $(INCLUDE) -c util.cpp

ivdata_util.o: ivdata_util.cpp
	$(CC) $(FLAGS) $(INCLUDE) -c ivdata_util.cpp

clean:
	rm -f *.o $(EXENAME) lex.openiv.c openivparser.tab.cpp \
	openivparser.tab.hpp

openivparser.tab.cpp openivparser.tab.hpp: openivparser.ypp
	bison -d openivparser.ypp

lex.openiv.c: openivlexer.lex
	flex openivlexer.lex

.PHONY: all clean

