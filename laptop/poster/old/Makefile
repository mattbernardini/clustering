   # Makefile for breadth-first search
# Sharlee Climer
# October, 2007
#-I/usr/mpi/gcc/openmpi-1.4.6/include -L/usr/mpi/gcc/openmpi-1.4.6/lib64 -lmpi spaghetti.cu -o progra


CC	= g++
CFLAGS 	= -g -Wall -std=c++17 -O3
LDFLAGS	= -lm
TARGET	= bfs
OBJS	= bfsNet.o network.o
TARGET2 = m
OBJS2 	= bfs.o Vertex.o Network.o Edge.o cluster.o base_logger.o gml_writer.o
VALGRIND = valgrind --tool=memcheck --verbose --leak-check=full --track-origins=yes --show-leak-kinds=all

$(TARGET2): $(OBJS2)
	$(CC) -o $(TARGET2) $(OBJS2) $(LDFLAGS)

$(TARGET):	$(OBJS)
		gcc -o $(TARGET) $(OBJS) $(LDFLAGS)
valgrind: $(TARGET2)
	$(VALGRIND) ./$(TARGET2) testFile.gml
bfsNet.o:	bfsNet.cpp bfsNet.h timer.h
		$(CC) $(CFLAGS) -c bfsNet.cpp

network.o:	network.cpp network.h
		$(CC) $(CFLAGS) -c network.cpp
bfs.o: bfs.cpp
		$(CC) $(CFLAGS) -c bfs.cpp
Vertex.o:	Vertex.cpp Vertex.h
		$(CC) $(CFLAGS) -c Vertex.cpp
Network.o:	Network.cpp Network.h
		$(CC) $(CFLAGS) -c Network.cpp
Edge.o:	Edge.cpp Edge.h Vertex.h
		$(CC) $(CFLAGS) -c Edge.cpp
base_logger.o: base_logger.cpp
		$(CC) $(CFLAGS) -c base_logger.cpp
cluster.o: cluster.cpp cluster.h
		$(CC) $(CFLAGS) -c cluster.cpp
gml_writer.o: gml_writer.cpp
		$(CC) $(CFLAGS) -c gml_writer.cpp
clean:
		/bin/rm -f *.o *.log core $(TARGET) $(TARGET2)
