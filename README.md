# 局域网麻将游戏 - C语言版本

## 项目概述
- **语言**: C语言（服务器端），Kotlin（Android客户端）
- **目的**: 学习C语言 + 网络编程 + 游戏开发
- **架构**: 客户端-服务器模型
- **网络**: TCP Socket通信

## 学习目标

### C语言技能
1. 指针和内存管理
2. 结构体和数据结构
3. 文件I/O操作
4. 网络Socket编程
5. 多线程/多进程编程
6. Makefile构建系统

### 项目技能
1. 游戏状态机设计
2. 网络协议设计
3. 客户端-服务器通信
4. 错误处理和调试
5. 性能优化

## 项目结构

```
mahjong-c-project/
├── src/                    # C源代码
│   ├── server/            # 服务器代码
│   │   ├── main.c         # 服务器主程序
│   │   ├── network.c      # 网络通信模块
│   │   ├── game_logic.c   # 游戏逻辑
│   │   ├── player.c       # 玩家管理
│   │   └── room.c         # 房间管理
│   ├── common/            # 公共代码
│   │   ├── tile.c         # 麻将牌定义
│   │   ├── protocol.c     # 通信协议
│   │   └── utils.c        # 工具函数
│   └── client/            # C测试客户端（可选）
├── include/               # 头文件
│   ├── server/
│   ├── common/
│   └── client/
├── lib/                   # 第三方库
├── test/                  # 测试代码
├── build/                 # 构建目录
├── bin/                   # 可执行文件
├── config/                # 配置文件
└── docs/                  # 文档
```

## 开发环境配置

### 1. 安装基础工具
```bash
sudo apt update
sudo apt install -y build-essential gdb valgrind git cmake
```

### 2. 安装开发库
```bash
sudo apt install -y libssl-dev libjson-c-dev libsqlite3-dev
```

### 3. 配置编辑器
推荐使用：
- **VSCode** + C/C++扩展
- **CLion**（JetBrains的C/C++ IDE）
- **Vim/Neovim** + coc.nvim

### 4. 构建和运行
```bash
# 使用Makefile
make all        # 编译所有
make server     # 编译服务器
make test       # 运行测试
make clean      # 清理

# 运行服务器
./bin/mahjong_server
```

## 学习路线

### 阶段1：C语言基础（1-2周）
- 复习C语言语法
- 指针和内存管理练习
- 结构体和文件操作

### 阶段2：网络编程基础（2-3周）
- Socket编程入门
- TCP客户端/服务器
- 多客户端处理

### 阶段3：游戏逻辑实现（3-4周）
- 麻将规则实现
- 游戏状态机
- 数据序列化

### 阶段4：完整项目（4-5周）
- Android客户端开发
- 联机测试
- 性能优化

## 通信协议设计

### 基础消息格式（二进制）
```c
struct message_header {
    uint32_t type;      // 消息类型
    uint32_t length;    // 数据长度
    uint32_t checksum;  // 校验和
};

// 消息类型定义
#define MSG_CREATE_ROOM  0x01
#define MSG_JOIN_ROOM    0x02
#define MSG_PLAY_TILE    0x03
#define MSG_GAME_STATE   0x04
```

### 文本协议（JSON over TCP）
```json
{
  "type": "play_tile",
  "player_id": "player_001",
  "tile": {"suit": "characters", "value": 5}
}
```

## 调试技巧

### 内存调试
```bash
# 使用valgrind检查内存泄漏
valgrind --leak-check=full ./bin/mahjong_server

# 使用gdb调试
gdb ./bin/mahjong_server
```

### 网络调试
```bash
# 查看端口占用
netstat -tulpn | grep :8000

# 使用telnet测试连接
telnet localhost 8000

# 使用nc发送测试数据
echo '{"type":"test"}' | nc localhost 8000
```

## 常见问题

### C语言特定问题
1. **内存泄漏**：使用valgrind定期检查
2. **缓冲区溢出**：严格检查数组边界
3. **指针错误**：初始化指针，避免野指针
4. **线程安全**：使用互斥锁保护共享数据

### 网络编程问题
1. **连接断开**：实现心跳机制
2. **数据粘包**：设计消息边界
3. **并发处理**：使用select/poll/epoll

## 资源推荐

### 学习资料
1. **书籍**：《C程序设计语言》、《Unix网络编程》
2. **在线**：C语言中文网、菜鸟教程
3. **视频**：B站C语言教程、网络编程教程

### 参考项目
1. **简单聊天室**：学习基础Socket编程
2. **五子棋游戏**：学习游戏状态同步
3. **HTTP服务器**：学习协议解析

## 开始开发

### 第一步：环境配置
按照上面的步骤配置开发环境

### 第二步：Hello Socket
实现一个简单的TCP服务器和客户端

### 第三步：麻将核心逻辑
实现洗牌、发牌、基本规则

### 第四步：网络集成
将游戏逻辑和网络通信结合

### 第五步：Android客户端
开发手机端界面和网络连接