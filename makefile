CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -g

DS_SRC = data_structures/fibonacci_heap.cpp data_structures/union_find.cpp
MST_SRC = graph/mst_algo/kruskal.cpp graph/mst_algo/prim.cpp
GRAPH_SRC = graph/graph.cpp graph/tree.cpp
SERVER_OBJ = server.o
SRC = $(DS_SRC) $(MST_SRC) $(GRAPH_SRC)
OBJ = $(SRC:.cpp=.o)

TEST_SRC = tests/test_main.cpp tests/data_structures_tests.cpp
TEST_OBJ = $(TEST_SRC:.cpp=.o)

TARGET = server
TEST_TARGET = test_exe
TEST_RUN = test

.PHONY: run $(TEST_RUN) all clean

run: $(TARGET)
	./$(TARGET)

all: $(TARGET) $(TEST_RUN)

$(TARGET): $(SERVER_OBJ) $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(TEST_RUN): $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJ) $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET) $(TEST_OBJ) $(TEST_TARGET) $(SERVER_OBJ)