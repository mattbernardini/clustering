########################################################################################################
# C++ Build Section
CPPFLAGS 	+= -g -Wall -std=c++17 -Ofast
LDFLAGS	= -lm
TARGET = m
OBJS 	= bfs.o vertex.o network.o edge.o cluster.o base_logger.o gml_writer.o

$(TARGET): $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LDFLAGS)
network.o:	network.cc network.h
		$(CXX) $(CPPFLAGS) -c network.cc
bfs.o: bfs.cc
		$(CXX) $(CPPFLAGS) -c bfs.cc
vertex.o:	vertex.cc vertex.h
		$(CXX) $(CPPFLAGS) -c vertex.cc
edge.o:	edge.cc edge.h vertex.h
		$(CXX) $(CPPFLAGS) -c edge.cc
base_logger.o: base_logger.cc
		$(CXX) $(CPPFLAGS) -c base_logger.cc
cluster.o: cluster.cc cluster.h
		$(CXX) $(CPPFLAGS) -c cluster.cc
gml_writer.o: gml_writer.cc
		$(CXX) $(CPPFLAGS) -c gml_writer.cc
########################################################################################################
# NVCC Build Section

NVCC = nvcc




##########################################################################################################
# Test setup and builds

# Valgrind
VALGRIND = valgrind --tool=memcheck --verbose --leak-check=full --track-origins=yes --show-leak-kinds=all

valgrind: $(TARGET)
	$(VALGRIND) ./$(TARGET) testFile.gml

# Google testing suite setup
GTEST_DIR = gtest
CPPFLAGS += -isystem $(GTEST_DIR)/include
CXXFLAGS += -g -Wall -Wextra -pthread
GTEST_HEADERS = $(GTEST_DIR)/gtest/*.h
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

gtest-all.o: $(GTEST_SRCS_) 
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
		$(GTEST_DIR)/gtest/gtest-all.cc

gtest_main.o: $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
		$(GTEST_DIR)/src/gtest_main.cc

gtest.a: gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

gtest_main.a: gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $@ $^
	
	# Individual tests

stateTest.o: tests/state/stateTest.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c ./tests/state/stateTest.cc

generatorTest.o: tests/stateGenerator/generatorTest.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c ./tests/stateGenerator/generatorTest.cc

stateTest: gtest_main.a stateTest.o state.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@	

generatorTest: gtest_main.a generatorTest.o state.o stateGenerator.o logger.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@	

testall: stateTest generatorTest
	./stateTest
	./generatorTest







clean:
		/bin/rm -f *.o *.log core *.gml $(TARGET)
