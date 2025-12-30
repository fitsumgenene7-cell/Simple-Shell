# Simple Makefile for OShell

CC      := gcc
CFLAGS  := -Wall -Wextra -Werror -Iinclude

TARGET  := oshell

# Source files
SRCS := src/main.c \
        src/shell.c \
        src/parser.c \
        src/execute.c \
        src/builtins.c \
        src/utils.c \
        src/error.c \
        src/expand.c \
        src/path.c \
        src/signals.c

# Object files in obj/ directory
OBJS := $(patsubst src/%.c,obj/%.o,$(SRCS))

# Ensure obj/ directory exists
$(shell mkdir -p obj)

# Default target
all: $(TARGET)

# Build executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

# Compile source files to object files in obj/
obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -f $(TARGET) $(OBJS)
	rm -rf obj

# Test the shell
test: $(TARGET)
	@echo "Testing basic commands..."
	@echo "echo 'Hello World'" | ./$(TARGET) 2>/dev/null | grep -q "Hello World" && echo "✓ echo command works" || echo "✗ echo command failed"
	@echo "exit 0" | ./$(TARGET) 2>/dev/null && echo "✓ exit command works" || echo "✗ exit command failed"

# Help target
help:
	@echo "Available targets:"
	@echo "  all   - Build the shell (default)"
	@echo "  clean - Remove all build files"
	@echo "  test  - Run basic tests"
	@echo "  help  - Show this help message"

.PHONY: all clean test help