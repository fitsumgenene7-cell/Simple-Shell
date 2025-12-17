# ============================================================================
# OShell - Unix Shell Implementation Makefile
# ============================================================================
# Compiler and Flags
# ============================================================================
CC      := gcc
CFLAGS  := -Wall -Wextra -Werror -Iinclude
LDFLAGS :=
TARGET  := oshell

# ============================================================================
# Directory Structure
# ============================================================================
SRC_DIR   := src
OBJ_DIR   := obj
BIN_DIR   := .
MAN_DIR   := man
TEST_DIR  := tests

# ============================================================================
# Source Files (Organized by module)
# ============================================================================
CORE_SRCS := $(SRC_DIR)/main.c \
             $(SRC_DIR)/shell.c

PARSE_SRCS := $(SRC_DIR)/parser.c

EXEC_SRCS := $(SRC_DIR)/execute.c

BUILTIN_SRCS := $(SRC_DIR)/builtins.c

UTIL_SRCS := $(SRC_DIR)/utils.c \
             $(SRC_DIR)/error.c \
             $(SRC_DIR)/expand.c \
             $(SRC_DIR)/path.c \
             $(SRC_DIR)/signals.c

# Combine all sources
SRCS := $(CORE_SRCS) $(PARSE_SRCS) $(EXEC_SRCS) $(BUILTIN_SRCS) $(UTIL_SRCS)

# ============================================================================
# Object Files
# ============================================================================
OBJS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

# ============================================================================
# Build Targets
# ============================================================================
.PHONY: all debug release clean distclean help test install

# Default target - build release version
all: release

# Release build (optimized)
release: CFLAGS += -O2 -DNDEBUG
release: $(BIN_DIR)/$(TARGET)

# Debug build (with symbols)
debug: CFLAGS += -g -DDEBUG -O0
debug: $(BIN_DIR)/$(TARGET)

# Link executable
$(BIN_DIR)/$(TARGET): $(OBJS)
	@echo "  LD      $@"
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJS)
	@echo "Build successful: $(TARGET)"

# Compile source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "  CC      $<"
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

# ============================================================================
# Dependencies (Optional - for more robust builds)
# ============================================================================
DEPDIR := $(OBJ_DIR)/.deps
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.d

COMPILE.c = $(CC) $(DEPFLAGS) $(CFLAGS) -c

# Include auto-generated dependencies
include $(wildcard $(DEPDIR)/*.d)

# Pattern rule with dependencies
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(DEPDIR)/%.d | $(DEPDIR)
	@echo "  CC      $<"
	$(COMPILE.c) $< -o $@

# Create dependency directory and files
$(DEPDIR): ; @mkdir -p $@
$(DEPDIR)/%.d: ;

# ============================================================================
# Utility Targets
# ============================================================================
# Clean build artifacts
clean:
	@echo "Cleaning build artifacts..."
	rm -rf $(OBJ_DIR) $(DEPDIR)

# Clean everything including executable
distclean: clean
	@echo "Cleaning everything..."
	rm -f $(BIN_DIR)/$(TARGET)

# Run basic tests
test: $(BIN_DIR)/$(TARGET)
	@echo "Running basic tests..."
	@echo "Test 1: Simple echo command"
	@echo "echo 'Hello World'" | ./$(TARGET) 2>/dev/null | grep -q "Hello World" && echo "✓ Test 1 passed" || echo "✗ Test 1 failed"
	@echo "Test 2: Exit command"
	@echo "exit 0" | ./$(TARGET) 2>/dev/null && echo "✓ Test 2 passed" || echo "✗ Test 2 failed"

# Create man pages (placeholder)
man:
	@echo "Creating man pages..."
	@mkdir -p $(MAN_DIR)
	@echo "Man pages would be created here"

# Install (for system-wide installation - optional)
install: $(BIN_DIR)/$(TARGET) man
	@echo "Installing $(TARGET)..."
	@echo "Not implemented - specify installation directory"

# Help
help:
	@echo "OShell Build System"
	@echo "==================="
	@echo "Available targets:"
	@echo "  all       - Build optimized release version (default)"
	@echo "  debug     - Build with debug symbols"
	@echo "  release   - Build optimized version"
	@echo "  clean     - Remove object files and dependencies"
	@echo "  distclean - Remove all build artifacts including executable"
	@echo "  test      - Run basic tests"
	@echo "  man       - Generate man pages"
	@echo "  install   - Install system-wide (not implemented)"
	@echo "  help      - Show this help"
	@echo ""
	@echo "Examples:"
	@echo "  make              # Build release version"
	@echo "  make debug        # Build debug version"
	@echo "  make clean        # Clean build files"
	@echo "  make test         # Run tests"

# ============================================================================
# File Presence Checks
# ============================================================================
# Check if required directories exist
$(shell mkdir -p $(SRC_DIR) $(OBJ_DIR) $(BIN_DIR) 2>/dev/null)

# ============================================================================
# Informational Display
# ============================================================================
$(info ==========================================)
$(info Building OShell - Unix Shell Implementation)
$(info ==========================================)
$(info Source files: $(words $(SRCS)))
$(info Object files: $(words $(OBJS)))
$(info Build type: $(if $(findstring debug,$(MAKECMDGOALS)),Debug,Release))
$(info ==========================================)