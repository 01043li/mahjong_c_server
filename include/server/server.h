/**
 * 麻将服务器头文件
 */
#ifndef SERVER_H
#define SERVER_H

#include <stdbool.h>

// 服务器配置
typedef struct {
    int port;               // 监听端口
    int max_clients;        // 最大客户端数
    int buffer_size;        // 缓冲区大小
    bool verbose;           // 详细日志
} ServerConfig;

// 服务器状态
typedef struct {
    int socket_fd;          // 服务器socket
    bool running;           // 运行状态
    int client_count;       // 当前客户端数
} ServerState;

// 函数声明

// 服务器初始化
bool server_init(ServerConfig* config);
bool server_start(ServerConfig* config);
void server_stop(void);

// 服务器运行
void server_run(void);

// 配置管理
ServerConfig server_default_config(void);
bool server_load_config(const char* filename, ServerConfig* config);
bool server_save_config(const char* filename, const ServerConfig* config);

// 状态查询
bool server_is_running(void);
int server_get_client_count(void);

#endif // SERVER_H