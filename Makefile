# Compiler and linker
CC = g++
CFLAGS = -I include -I /ucrt64/include/SDL2 -I /ucrt64/include/GLM
LDFLAGS = -L /ucrt64/lib -lmingw32 -lSDL2main -lSDL2 -lopengl32 -lglew32

# Directories
SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include

# Target binary
TARGET = $(BUILD_DIR)/game.exe

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# Object files
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Build rules
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)/*.o $(TARGET)
