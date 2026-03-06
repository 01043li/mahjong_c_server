# 麻将游戏C语言项目Makefile

CC = gcc
CFLAGS = -Wall -Wextra -g -I./include
LDFLAGS = -lpthread -ljson-c -lsqlite3 -lssl -lcrypto

# 目录定义
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin
INCLUDE_DIR = include

# 源文件
SERVER_SRCS = $(SRC_DIR)/server/main.c \
              $(SRC_DIR)/server/network.c \
              $(SRC_DIR)/server/game_logic.c \
              $(SRC_DIR)/server/player.c \
              $(SRC_DIR)/server/room.c \
              $(SRC_DIR)/common/tile.c \
              $(SRC_DIR)/common/protocol.c \
              $(SRC_DIR)/common/utils.c

TEST_SRCS = $(SRC_DIR)/test/test_tile.c \
            $(SRC_DIR)/test/test_protocol.c

# 目标文件
SERVER_OBJS = $(SERVER_SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
TEST_OBJS = $(TEST_SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# 可执行文件
SERVER_BIN = $(BIN_DIR)/mahjong_server
TEST_BIN = $(BIN_DIR)/test_runner

# 默认目标
all: directories $(SERVER_BIN) $(TEST_BIN)

# 创建目录
directories:
	@mkdir -p $(BUILD_DIR)/server
	@mkdir -p $(BUILD_DIR)/common
	@mkdir -p $(BUILD_DIR)/test
	@mkdir -p $(BIN_DIR)

# 编译服务器
$(SERVER_BIN): $(SERVER_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# 编译测试
$(TEST_BIN): $(TEST_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# 编译规则
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# 清理
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# 运行服务器
run: $(SERVER_BIN)
	./$(SERVER_BIN)

# 运行测试
test: $(TEST_BIN)
	./$(TEST_BIN)

# 调试
debug: CFLAGS += -DDEBUG -O0
debug: clean all

# 发布版本
release: CFLAGS = -Wall -O2 -I./include
release: clean all

# 内存检查
memcheck: $(SERVER_BIN)
	valgrind --leak-check=full ./$(SERVER_BIN)

# 静态分析
analyze:
	cppcheck --enable=all --inconclusive $(SRC_DIR)

# 帮助
help:
	@echo "可用命令:"
	@echo "  make all        - 编译所有目标（默认）"
	@echo "  make clean      - 清理构建文件"
	@echo "  make run        - 编译并运行服务器"
	@echo "  make test       - 编译并运行测试"
	@echo "  make debug      - 编译调试版本"
	@echo "  make release    - 编译发布版本"
	@echo "  make memcheck   - 运行内存检查"
	@echo "  make analyze    - 静态代码分析"
	@echo "  make help       - 显示此帮助"

.PHONY: all clean run test debug release memcheck analyze help directories