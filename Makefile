CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O0 -g -Iinclude

SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:src/%.cpp=build/%.o)
BIN = $(SRC:src/%.cpp=bin/%)

all: $(BIN)

$(BIN): $(OBJ)
	mkdir -p bin
	$(CXX) $(CXXFLAGS) $^ -o $@

build/%.o: src/%.cpp
	mkdir -p build
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf bin/ build/

.PHONY: all clean