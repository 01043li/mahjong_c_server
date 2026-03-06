/**
 * 网络通信模块
 */
#include "server/server.h"
#include "common/protocol.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

// 服务器状态
static ServerState server_state = {0};
static ServerConfig server_config;

// 默认配置
ServerConfig server_default_config(void) {
    ServerConfig config = {
        .port = 8888,
        .max_clients = 100,
        .buffer_size = 4096,
        .verbose = true
    };
    return config;
}

// 服务器初始化
bool server_init(ServerConfig* config) {
    if (!config) return false;
    
    server_config = *config;
    server_state.running = false;
    server_state.client_count = 0;
    
    return true;
}

// 创建服务器socket
static int create_server_socket(int port) {
    int server_fd;
    struct sockaddr_in address;
    int opt = 1;
    
    // 创建socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket创建失败");
        return -1;
    }
    
    // 设置socket选项
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt失败");
        close(server_fd);
        return -1;
    }
    
    // 绑定地址
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind失败");
        close(server_fd);
        return -1;
    }
    
    // 监听
    if (listen(server_fd, 10) < 0) {
        perror("listen失败");
        close(server_fd);
        return -1;
    }
    
    return server_fd;
}

// 启动服务器
bool server_start(ServerConfig* config) {
    if (!config) config = &server_config;
    
    // 创建服务器socket
    server_state.socket_fd = create_server_socket(config->port);
    if (server_state.socket_fd < 0) {
        return false;
    }
    
    server_state.running = true;
    
    if (config->verbose) {
        printf("服务器启动成功，监听端口 %d\n", config->port);
        printf("等待客户端连接...\n");
    }
    
    return true;
}

// 停止服务器
void server_stop(void) {
    server_state.running = false;
    
    if (server_state.socket_fd > 0) {
        close(server_state.socket_fd);
        server_state.socket_fd = -1;
    }
    
    printf("服务器已停止\n");
}

// 处理客户端连接（简化版）
static void* handle_client(void* arg) {
    int client_fd = *(int*)arg;
    free(arg);
    
    char buffer[1024];
    int bytes_read;
    
    // 发送欢迎消息
    const char* welcome = "欢迎连接到麻将游戏服务器！\n";
    send(client_fd, welcome, strlen(welcome), 0);
    
    // 简单echo服务器
    while ((bytes_read = recv(client_fd, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytes_read] = '\0';
        
        if (server_config.verbose) {
            printf("收到客户端消息: %s", buffer);
        }
        
        // 回显消息
        send(client_fd, buffer, bytes_read, 0);
        
        // 如果是退出命令
        if (strncmp(buffer, "quit", 4) == 0 || strncmp(buffer, "exit", 4) == 0) {
            break;
        }
    }
    
    close(client_fd);
    return NULL;
}

// 运行服务器主循环
void server_run(void) {
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    
    while (server_state.running) {
        // 接受客户端连接
        int client_fd = accept(server_state.socket_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        
        if (client_fd < 0) {
            if (server_state.running) {
                perror("accept失败");
            }
            continue;
        }
        
        // 打印客户端信息
        if (server_config.verbose) {
            printf("新客户端连接: %s:%d\n", 
                   inet_ntoa(address.sin_addr), 
                   ntohs(address.sin_port));
        }
        
        // 创建线程处理客户端（简化版，实际应该用线程池）
        pthread_t thread_id;
        int* client_fd_ptr = malloc(sizeof(int));
        *client_fd_ptr = client_fd;
        
        if (pthread_create(&thread_id, NULL, handle_client, client_fd_ptr) != 0) {
            perror("线程创建失败");
            free(client_fd_ptr);
            close(client_fd);
            continue;
        }
        
        // 分离线程
        pthread_detach(thread_id);
        
        server_state.client_count++;
    }
}

// 状态查询
bool server_is_running(void) {
    return server_state.running;
}

int server_get_client_count(void) {
    return server_state.client_count;
}

// 配置管理（简化版）
bool server_load_config(const char* filename, ServerConfig* config) {
    // TODO: 从文件加载配置
    *config = server_default_config();
    return true;
}

bool server_save_config(const char* filename, const ServerConfig* config) {
    // TODO: 保存配置到文件
    return true;
}