# variables
CXX = g++
CXXFLAGS = -Wall -std=c++17
LDFLAGS = -lSDL2
SRC = src/main.cpp src/cpu.cpp src/graphics.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = space_invaders

# default rule
all: $(TARGET)

# compile
$(TARGET): $(OBJ)
	$(CXX) -o $@ $(OBJ) $(LDFLAGS)

# compile objects
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# clean
clean:
	rm -f $(OBJ) $(TARGET)
