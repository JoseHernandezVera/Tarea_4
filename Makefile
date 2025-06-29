CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2
SRC = main.cpp Map.cpp DFSGenerator.cpp PrimGenerator.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = mapgen

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f $(OBJ) $(TARGET)
