# Makefile for breadth-first search
# Sharlee Climer
# October, 2007
#-I/usr/mpi/gcc/openmpi-1.4.6/include -L/usr/mpi/gcc/openmpi-1.4.6/lib64 -lmpi spaghetti.cu -o progra


CC	= mpic++
CFLAGS 	= -g -Wall -std=c++17 -O3
LDFLAGS	= -lm
TARGET2 = m
OBJS2 	= bfs.o vertex.o network.o edge.o cluster.o base_logger.o gml_writer.o
VALGRIND = valgrind --tool=memcheck --verbose --leak-check=full --track-origins=yes --show-leak-kinds=all

$(TARGET2): $(OBJS2)
	$(CC) -o $(TARGET2) $(OBJS2) $(LDFLAGS)

valgrind: $(TARGET2)
	$(VALGRIND) ./$(TARGET2) testFile.gml
network.o:	network.cc network.h
		$(CC) $(CFLAGS) -c network.cc
bfs.o: bfs.cc
		$(CC) $(CFLAGS) -c bfs.cc
vertex.o:	vertex.cc vertex.h
		$(CC) $(CFLAGS) -c vertex.cc
network.o:	network.cc network.h
		$(CC) $(CFLAGS) -c network.cc
edge.o:	edge.cc edge.h vertex.h
		$(CC) $(CFLAGS) -c edge.cc
base_logger.o: base_logger.cc
		$(CC) $(CFLAGS) -c base_logger.cc
cluster.o: cluster.cc cluster.h
		$(CC) $(CFLAGS) -c cluster.cc
gml_writer.o: gml_writer.cc
		$(CC) $(CFLAGS) -c gml_writer.cc
clean:
		/bin/rm -f *.o *.log core $(TARGET) $(TARGET2)
