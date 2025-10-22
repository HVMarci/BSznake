# Compiler and flags
CC      := gcc
#CFLAGS  := -Wall -Wextra -Werror -Iinclude $(shell sdl2-config --cflags)
CFLAGS  := -Wall -Wextra -Iinclude -IC:\MinGW\w64devkit\include\SDL2
#LDFLAGS := $(shell sdl2-config --libs)
LDFLAGS := -LC:\MinGW\w64devkit\lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_gfx -lSDL2_ttf# -mwindows

# Directories
SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

# Target
TARGET  := $(BIN_DIR)/game

# Files
SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Default rule
all: $(TARGET)

# Link
$(TARGET): $(OBJ)
	@mkdir -p $(BIN_DIR)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)
	@echo "Build complete: $@"

# Compile
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
	@echo "Cleaned all build files."

# Rebuild everything
rebuild: clean all

# Run the program
run: $(TARGET)
	./$(TARGET)

# Phony targets
.PHONY: all clean rebuild run
