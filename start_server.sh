#!/bin/bash

# 麻将服务器启动脚本

echo "=========================================="
echo "       麻将游戏服务器启动脚本"
echo "=========================================="

# 颜色定义
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# 项目目录
PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SERVER_BIN="$PROJECT_DIR/bin/mahjong_server"
PID_FILE="$PROJECT_DIR/server.pid"
LOG_FILE="$PROJECT_DIR/server.log"

# 默认配置
PORT=8888
MAX_CLIENTS=100
VERBOSE=false

# 显示帮助
show_help() {
    echo "用法: $0 [选项]"
    echo ""
    echo "选项:"
    echo "  --port PORT          服务器端口 (默认: 8888)"
    echo "  --max-clients NUM    最大客户端数 (默认: 100)"
    echo "  --verbose            详细日志输出"
    echo "  --stop               停止服务器"
    echo "  --status             查看服务器状态"
    echo "  --restart            重启服务器"
    echo "  --log                查看服务器日志"
    echo "  --help               显示此帮助信息"
    echo ""
    echo "示例:"
    echo "  $0                    # 使用默认配置启动"
    echo "  $0 --port 9999        # 指定端口启动"
    echo "  $0 --stop             # 停止服务器"
    echo "  $0 --status           # 查看状态"
}

# 检查服务器是否运行
check_running() {
    if [ -f "$PID_FILE" ]; then
        PID=$(cat "$PID_FILE")
        if kill -0 $PID 2>/dev/null; then
            return 0  # 正在运行
        else
            rm -f "$PID_FILE"
            return 1  # PID文件存在但进程不存在
        fi
    fi
    return 1  # 未运行
}

# 启动服务器
start_server() {
    echo -e "${BLUE}检查服务器状态...${NC}"
    
    if check_running; then
        echo -e "${RED}❌ 服务器已在运行 (PID: $PID)${NC}"
        return 1
    fi
    
    echo -e "${BLUE}编译服务器...${NC}"
    cd "$PROJECT_DIR"
    
    # 清理并编译
    make clean > /dev/null 2>&1
    if ! make > /dev/null 2>&1; then
        echo -e "${RED}❌ 编译失败${NC}"
        echo "请运行 'make' 查看详细错误"
        return 1
    fi
    
    echo -e "${GREEN}✅ 编译成功${NC}"
    
    # 构建启动命令
    CMD="$SERVER_BIN --port $PORT --max-clients $MAX_CLIENTS"
    if [ "$VERBOSE" = true ]; then
        CMD="$CMD --verbose"
    fi
    
    echo -e "${BLUE}启动服务器...${NC}"
    echo "端口: $PORT"
    echo "最大客户端: $MAX_CLIENTS"
    echo "命令: $CMD"
    echo ""
    
    # 启动服务器
    if [ "$VERBOSE" = true ]; then
        $CMD &
    else
        $CMD > "$LOG_FILE" 2>&1 &
    fi
    
    SERVER_PID=$!
    echo $SERVER_PID > "$PID_FILE"
    
    # 等待服务器启动
    sleep 2
    
    if check_running; then
        echo -e "${GREEN}✅ 服务器启动成功 (PID: $SERVER_PID)${NC}"
        echo -e "${BLUE}监听端口:${NC} $PORT"
        echo -e "${BLUE}日志文件:${NC} $LOG_FILE"
        echo -e "${BLUE}PID文件:${NC} $PID_FILE"
        
        # 显示监听状态
        echo ""
        echo -e "${YELLOW}网络状态:${NC}"
        if netstat -tlnp 2>/dev/null | grep ":$PORT" > /dev/null; then
            echo -e "${GREEN}✅ 端口 $PORT 正在监听${NC}"
        else
            echo -e "${YELLOW}⚠️  端口状态检查失败，但进程在运行${NC}"
        fi
        
        # 测试连接
        echo ""
        echo -e "${YELLOW}连接测试:${NC}"
        if timeout 2 bash -c "echo -n '' > /dev/tcp/localhost/$PORT" 2>/dev/null; then
            echo -e "${GREEN}✅ 本地连接测试成功${NC}"
        else
            echo -e "${YELLOW}⚠️  本地连接测试失败${NC}"
        fi
    else
        echo -e "${RED}❌ 服务器启动失败${NC}"
        echo "请检查日志文件: $LOG_FILE"
        rm -f "$PID_FILE"
        return 1
    fi
    
    return 0
}

# 停止服务器
stop_server() {
    echo -e "${BLUE}停止服务器...${NC}"
    
    if check_running; then
        echo -e "${YELLOW}正在停止服务器 (PID: $PID)...${NC}"
        kill $PID
        
        # 等待进程结束
        for i in {1..10}; do
            if ! kill -0 $PID 2>/dev/null; then
                break
            fi
            sleep 1
            echo -n "."
        done
        echo ""
        
        if kill -0 $PID 2>/dev/null; then
            echo -e "${YELLOW}强制终止...${NC}"
            kill -9 $PID
        fi
        
        rm -f "$PID_FILE"
        echo -e "${GREEN}✅ 服务器已停止${NC}"
    else
        echo -e "${YELLOW}⚠️  服务器未运行${NC}"
        rm -f "$PID_FILE" 2>/dev/null
    fi
}

# 查看状态
status_server() {
    echo -e "${BLUE}服务器状态检查...${NC}"
    
    if check_running; then
        echo -e "${GREEN}✅ 服务器正在运行${NC}"
        echo "PID: $PID"
        echo "端口: $PORT"
        echo "运行时间: $(ps -o etime= -p $PID | xargs)"
        
        # 检查端口监听
        echo ""
        echo -e "${YELLOW}端口监听状态:${NC}"
        netstat -tlnp 2>/dev/null | grep ":$PORT" || echo "未找到端口监听"
        
        # 检查连接数
        echo ""
        echo -e "${YELLOW}连接统计:${NC}"
        ss -s 2>/dev/null | grep -A 10 "Total:" || echo "连接统计不可用"
    else
        echo -e "${RED}❌ 服务器未运行${NC}"
    fi
}

# 查看日志
view_log() {
    if [ -f "$LOG_FILE" ]; then
        echo -e "${BLUE}服务器日志 (最后50行):${NC}"
        echo "=========================================="
        tail -50 "$LOG_FILE"
        echo "=========================================="
        echo "完整日志: $LOG_FILE"
    else
        echo -e "${YELLOW}⚠️  日志文件不存在${NC}"
    fi
}

# 重启服务器
restart_server() {
    echo -e "${BLUE}重启服务器...${NC}"
    stop_server
    sleep 2
    start_server
}

# 解析命令行参数
while [[ $# -gt 0 ]]; do
    case $1 in
        --port)
            PORT="$2"
            shift 2
            ;;
        --max-clients)
            MAX_CLIENTS="$2"
            shift 2
            ;;
        --verbose)
            VERBOSE=true
            shift
            ;;
        --stop)
            ACTION="stop"
            shift
            ;;
        --status)
            ACTION="status"
            shift
            ;;
        --restart)
            ACTION="restart"
            shift
            ;;
        --log)
            ACTION="log"
            shift
            ;;
        --help|-h)
            show_help
            exit 0
            ;;
        *)
            echo -e "${RED}❌ 未知选项: $1${NC}"
            show_help
            exit 1
            ;;
    esac
done

# 执行操作
case "${ACTION:-start}" in
    start)
        start_server
        ;;
    stop)
        stop_server
        ;;
    status)
        status_server
        ;;
    restart)
        restart_server
        ;;
    log)
        view_log
        ;;
esac

echo ""
echo -e "${BLUE}快速命令参考:${NC}"
echo "  启动: $0"
echo "  停止: $0 --stop"
echo "  状态: $0 --status"
echo "  重启: $0 --restart"
echo "  日志: $0 --log"
echo "  帮助: $0 --help"