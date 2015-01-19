###############################################################################
# CS/CNS 171 Fall 2014
#
# This is a template Makefile for the assignment. Edit it however you find
# convenient, though the only line that you will probably need to change for
# is the INCLUDE line.
###############################################################################
CC = g++
FLAGS = -g -o
# Change the following directory to whatever directory has your Eigen folder.
INCLUDE = -I/home/pwon111/Documents/repos/cs171/hw0-folder/lib/
SOURCES = *.cpp *.h

EXENAME = ppm_test

all: $(SOURCES)
	$(CC) $(FLAGS) $(EXENAME) $(INCLUDE) $(SOURCES)

clean:
	rm -f *.o $(EXENAME)

.PHONY: all clean

