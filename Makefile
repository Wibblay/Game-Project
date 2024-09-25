# Compiler and linker
CC = g++
CFLAGS = -I include -I /ucrt64/include/SDL2 -I /ucrt64/include/GLM
LDFLAGS = -L /ucrt64/lib -lmingw32 -lSDL2main -lSDL2 -lopengl32 -lglew32

# Directories
SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include
SHADERS_DIR = shaders

# Target binary
TARGET = $(BUILD_DIR)/game.exe

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# Object files
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Build rules
.PHONY: all clean copy_shaders

all: $(TARGET) copy_shaders

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

copy_shaders: | $(BUILD_DIR)
	xcopy /E /I /Y $(SHADERS_DIR) $(BUILD_DIR)\$(notdir $(SHADERS_DIR))\

clean:
	del /Q $(BUILD_DIR)\*.o $(TARGET)
	rd /S /Q $(BUILD_DIR)\$(notdir $(SHADERS_DIR))
