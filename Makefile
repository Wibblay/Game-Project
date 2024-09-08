# Compiler
CXX = g++
CXXFLAGS = -g -Iinclude

# Output executable
TARGET = build/main.exe

# Find all .cpp files in src/
SRC = $(wildcard src/*.cpp)

# Object files
OBJ = $(SRC:.cpp=.o)

# Default target
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJ)
	$(CXX) -o $@ $^ -Llib/SDL2 -Llib -lSDL2 -lglew32 -lopengl32 -mwindows

# Compile .cpp files into .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean target to remove object files and executable
clean:
	rm -f src/*.o $(TARGET)
