# Makefile for Mahjong Game Server

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g -I./include
LDFLAGS = -lpthread

# Directories
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin
INCLUDE_DIR = include

# Server source files
SERVER_SRCS = $(SRC_DIR)/server/main.c \
              $(SRC_DIR)/server/network.c \
              $(SRC_DIR)/server/game_logic.c \
              $(SRC_DIR)/server/player.c

# Server object files
SERVER_OBJS = $(SERVER_SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Client source files (if any)
CLIENT_SRCS = $(SRC_DIR)/client/main.c
CLIENT_OBJS = $(CLIENT_SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Executables
SERVER_EXEC = $(BIN_DIR)/mahjong_server
CLIENT_EXEC = $(BIN_DIR)/mahjong_client

# Default target
all: directories $(SERVER_EXEC)

# Create necessary directories
directories:
	@mkdir -p $(BUILD_DIR)/server
	@mkdir -p $(BUILD_DIR)/client
	@mkdir -p $(BIN_DIR)

# Build server
$(SERVER_EXEC): $(SERVER_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	@echo "✅ Server built: $@"

# Build client
client: directories $(CLIENT_EXEC)

$(CLIENT_EXEC): $(CLIENT_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	@echo "✅ Client built: $@"

# Compile server source files
$(BUILD_DIR)/server/%.o: $(SRC_DIR)/server/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compile client source files
$(BUILD_DIR)/client/%.o: $(SRC_DIR)/client/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
	@echo "🧹 Cleaned build directories"

# Run server
run: all
	@echo "🚀 Starting Mahjong Server..."
	./$(SERVER_EXEC)

# Debug server
debug: all
	@echo "🐛 Starting server with gdb..."
	gdb ./$(SERVER_EXEC)

# Install dependencies (Ubuntu/Debian)
install-deps:
	@echo "📦 Installing dependencies..."
	sudo apt-get update
	sudo apt-get install -y gcc make gdb valgrind

# Check code style
check:
	@echo "📝 Checking code style..."
	@find $(SRC_DIR) -name "*.c" -o -name "*.h" | xargs clang-format --dry-run --Werror 2>/dev/null || echo "Note: Install clang-format for style checking"

# Help
help:
	@echo "Mahjong Game Server Makefile"
	@echo ""
	@echo "Available targets:"
	@echo "  all        - Build server (default)"
	@echo "  client     - Build client"
	@echo "  clean      - Remove build files"
	@echo "  run        - Build and run server"
	@echo "  debug      - Build and debug server"
	@echo "  install-deps - Install build dependencies"
	@echo "  check      - Check code style"
	@echo "  help       - Show this help"
	@echo ""
	@echo "Examples:"
	@echo "  make              # Build server"
	@echo "  make run          # Build and run server"
	@echo "  make clean        # Clean build"
	@echo "  make install-deps # Install dependencies"

.PHONY: all clean run debug install-deps check help directories