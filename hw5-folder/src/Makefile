CC = g++
FLAGS = -std=c++11 -g -o
OFLAGS = -std=c++11 -g -c

INCLUDE = -I/usr/X11R6/include -I/usr/include/GL -I/usr/include -I../lib
LIBDIR = -L/usr/X11R6/lib -L/usr/local/lib
SOURCES = lex.yy.c graphicsparser.tab.c main.cpp main.hpp data.h util.o superquadrics.o
LIBS = -lGLEW -lGL -lGLU -lglut -lm

EXENAME = mesher

all: $(SOURCES)
	$(CC) $(FLAGS) $(EXENAME) $(INCLUDE) $(LIBDIR) $(SOURCES) $(LIBS)

superquadrics.o: superquadrics.cpp
	$(CC) $(OFLAGS) superquadrics.cpp $(INCLUDE) $(LIBDIR) $(LIBS)

util.o: util.cpp
	$(CC) $(OFLAGS) util.cpp $(INCLUDE) $(LIBDIR) $(LIBS)

graphicsparser.tab.c graphicsparser.tab.h: graphicsparser.y
	bison -d graphicsparser.y

lex.yy.c: graphicslexer.l
	flex graphicslexer.l

clean:
	rm -f *.o $(EXENAME) graphicsparser.tab.c graphicsparser.tab.h lex.yy.cpp

.PHONY: all clean

