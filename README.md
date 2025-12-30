
# 🐚 OShell - A Simplified Unix Command-Line Interpreter

**OShell** is a lightweight Unix-like shell implementation written in C, designed to demonstrate core operating system concepts including process management, command parsing, file I/O, and environment management. It replicates essential functionality from shells like `bash` while maintaining simplicity and educational clarity.

---

## 📋 Table of Contents
- [Features](#-features)
- [Project Structure](#-project-structure)
- [Installation & Compilation](#-installation--compilation)
- [Usage](#-usage)
- [Built-in Commands](#-built-in-commands)
- [Operators](#-operators)
- [Variable Expansion](#-variable-expansion)
- [Error Handling](#-error-handling)
- [Testing](#-testing)
- [Technical Details](#-technical-details)
- [Contributors](#-contributors)
- [License](#-license)

---

## ✨ Features

### **Invocation Modes**
- **Interactive Mode**: Interactive prompt (`$`) with command input
- **Batch File Mode**: Execute commands from a script file
- **Pipe Mode**: Read commands from standard input (non-interactive)

### **Command Parsing & Operators**
- **Sequential Execution** (`;`): Execute commands in sequence
- **Conditional AND** (`&&`): Execute second command only if first succeeds
- **Conditional OR** (`||`): Execute second command only if first fails
- **Parallel Execution** (`&`): Execute commands concurrently
- **Redirection** (`>`): Redirect stdout/stderr to a file (overwrites)
- **Comments** (`#`): Ignore text following `#` on a line

### **Built-in Commands**
- `cd` – Change directory
- `exit` – Exit the shell
- `env` – Display environment variables
- `setenv` – Set environment variable
- `unsetenv` – Unset environment variable
- `alias` – Define command aliases
- `path` – Set command search path

### **Advanced Features**
- Environment variable expansion (`$VAR`)
- Special variables: `$?` (last exit status), `$$` (shell PID)
- PATH-based command resolution
- Signal handling (Ctrl+C ignored, Ctrl+D exits)
- Whitespace normalization in commands

---


## 📁 Project Structure
oshell-project/
├── Makefile                    # Build configuration
├── README.md                   # This documentation
├── include/                    # Header files
│   ├── shell.h                # Main shell structures
│   ├── parser.h               # Parsing functions
│   ├── execute.h              # Execution functions
│   ├── builtins.h             # Built-in commands
│   ├── expand.h               # Variable expansion
│   ├── path.h                 # PATH management
│   ├── error.h                # Error handling
│   ├── utils.h                # Utilities
│   └── signals.h              # Signal handling
├── src/                       # Source code
│   ├── main.c                 # Entry point
│   ├── shell.c                # Main shell loop
│   ├── parser.c               # Command parsing
│   ├── execute.c              # Command execution
│   ├── builtins.c             # Built-in implementations
│   ├── expand.c               # Variable expansion
│   ├── path.c                 # PATH resolution
│   ├── error.c                # Error handling
│   ├── utils.c                # Utility functions
│   └── signals.c              # Signal handlers
├── man/                       # Manual pages
│   ├── exit.1
│   ├── cd.1
│   ├── env.1
│   ├── setenv.1
│   ├── unsetenv.1
│   ├── alias.1
│   └── path.1
└── tests/                     # Test files
    ├── test_commands.txt
    ├── test_batch.txt
    └── test_parser.txt


---

## 🔧 Installation & Compilation

### Prerequisites
- GCC compiler
- Unix-like operating system (Linux, macOS, WSL)
- Standard C library

### Compilation
```bash
# Clone/download the project
cd oshell-project/

# Compile the shell
make# Simple-Shell
