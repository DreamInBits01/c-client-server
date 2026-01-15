# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -Iinclude
LDFLAGS = 

# Directories
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
BIN_DIR = bin

# Target executable
TARGET = $(BIN_DIR)/server

# Source files
SRCS = src/main.c \
	src/connections/manager.c \
	src/connections/multiplexer.c \
	src/net/listener.c \
	src/net/utils.c \
	src/event_loop/index.c
# Object files
OBJS = $(SRCS:src/%.c=obj/%.o)

# Header dependencies
DEPS = include/connections/multiplexer.h \
	include/connections/structs.h \
	include/net/listener.h \
	include/net/structs.h \
	include/net/utils.h

# Default target
.PHONY: all
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LDFLAGS)
	@echo "Build successful: $(TARGET)"

# Pattern rule for object files
obj/%.o: src/%.c $(DEPS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
.PHONY: clean
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
	@echo "Cleaned build artifacts"

# Clean and rebuild
.PHONY: rebuild
rebuild: clean all

# Run the executable
.PHONY: run
run: $(TARGET)
	./$(TARGET)

# Debug build
.PHONY: debug
debug: CFLAGS += -g -O0 -DDEBUG
debug: clean all

# Release build
.PHONY: release
release: CFLAGS += -O3 -DNDEBUG
release: clean all

# Display help
.PHONY: help
help:
	@echo "Available targets:"
	@echo "  all      - Build the project (default)"
	@echo "  clean    - Remove build artifacts"
	@echo "  rebuild  - Clean and rebuild"
	@echo "  run      - Build and run the executable"
	@echo "  debug    - Build with debug symbols"
	@echo "  release  - Build optimized release version"
	@echo "  help     - Display this help message"