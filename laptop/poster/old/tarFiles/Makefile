# Makefile for extracting genotype data for a list of individuals
# Sharlee Climer
# July, 2010


CC	= g++
CFLAGS 	= -g
TARGET	= extractRows
OBJS	= extractIndiv.o

$(TARGET):	$(OBJS)
		$(CC) -o $(TARGET) $(OBJS)

extractIndiv.o:	extractIndiv.cpp extractIndiv.h timer.h
		$(CC) $(CFLAGS) -c extractIndiv.cpp

clean:
		/bin/rm -f *.o $(TARGET)
