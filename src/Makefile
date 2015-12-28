OBJS = main.o game.o graphics.o board.o utilities.o

# CC specifies which compiler we are using
CC=g++

# OBJ_NAME the name of the object to be compiled
OBJ_NAME = main

# IDIR include directory
IDIR = ../headers

DEBUG = 
STDFLAG = -std=c++11
INCLUDEFLAG = -I/usr/local/include -I../headers
LIBFLAG = -lSDL2 -lSDL2_image -lSDL2_mixer

LFLAGS = -Wall $(LIBFLAG)
CFLAGS = -Wall $(DEBUG) $(STDFLAG) $(INCLUDEFLAG)


main: $(OBJS)
	$(CC) $(LFLAGS) -o $(OBJ_NAME) $(OBJS)

# main.cpp
main.o: main.cpp game.cpp $(IDIR)/game.h
	$(CC) $(CFLAGS) -c main.cpp

# game.cpp
game.o: game.cpp $(IDIR)/game.h $(IDIR)/graphics.h $(IDIR)/globals.h $(IDIR)/board.h
	$(CC) $(CFLAGS) -c game.cpp

$(IDIR)/game.h:

# board.cpp
board.o: board.cpp $(IDIR)/board.h $(IDIR)/graphics.h $(IDIR)/utilities.h
	$(CC) $(CFLAGS) -c board.cpp

$(IDIR)/board.h: $(IDIR)/globals.h

# grahpics.cpp
graphics.o: graphics.cpp $(IDIR)/graphics.h
	$(CC) $(CFLAGS) -c graphics.cpp

$(IDIR)/graphics.h: 

# utilities.cpp
utilities.o: utilities.cpp $(IDIR)/utilities.h
	$(CC) $(CFLAGS) -c utilities.cpp

$(IDIR)/utilities.h:

# clean
clean:
	rm $(OBJS) $(OBJ_NAME)