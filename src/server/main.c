/**
 * 麻将服务器主程序
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include "server/server.h"
#include "common/tile.h"

// 全局服务器状态
static ServerState server_state = {0};

// 信号处理函数
static void signal_handler(int sig) {
    printf("\n收到信号 %d，正在关闭服务器...\n", sig);
    server_stop();
}

// 测试麻将牌功能
static void test_tile_functions(void) {
    printf("=== 测试麻将牌功能 ===\n");
    
    // 创建牌组
    TileSet* set = tileset_create(false);
    if (!set) {
        printf("创建牌组失败\n");
        return;
    }
    
    printf("创建牌组成功，总牌数: %d\n", set->total_count);
    
    // 洗牌
    tileset_shuffle(set);
    printf("洗牌完成\n");
    
    // 创建手牌
    Hand* hand = hand_create(20);
    if (!hand) {
        printf("创建手牌失败\n");
        tileset_destroy(set);
        return;
    }
    
    // 摸13张牌
    printf("摸13张牌:\n");
    for (int i = 0; i < 13; i++) {
        Tile tile = tileset_draw(set);
        hand_add_tile(hand, tile);
        printf("%s ", tile_to_string(&tile));
    }
    printf("\n");
    
    // 排序并显示手牌
    hand_sort(hand);
    hand_print(hand);
    
    printf("剩余牌数: %d\n", tileset_remaining(set));
    
    // 清理
    hand_destroy(hand);
    tileset_destroy(set);
    
    printf("测试完成\n");
}

// 主函数
int main(int argc, char* argv[]) {
    printf("=== 局域网麻将游戏服务器 ===\n");
    printf("版本: 1.0.0\n");
    printf("作者: L下生 (学习C语言项目)\n\n");
    
    // 注册信号处理
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    
    // 测试麻将牌功能
    test_tile_functions();
    
    // 默认配置
    ServerConfig config = server_default_config();
    
    // 解析命令行参数
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
            config.port = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-c") == 0 && i + 1 < argc) {
            config.max_clients = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-v") == 0) {
            config.verbose = true;
        } else if (strcmp(argv[i], "-h") == 0) {
            printf("用法: %s [选项]\n", argv[0]);
            printf("选项:\n");
            printf("  -p <端口>     服务器端口 (默认: 8888)\n");
            printf("  -c <数量>     最大客户端数 (默认: 100)\n");
            printf("  -v            详细输出\n");
            printf("  -h            显示此帮助\n");
            return 0;
        }
    }
    
    printf("服务器配置:\n");
    printf("  端口: %d\n", config.port);
    printf("  最大客户端数: %d\n", config.max_clients);
    printf("  详细日志: %s\n", config.verbose ? "是" : "否");
    
    // 初始化服务器
    if (!server_init(&config)) {
        fprintf(stderr, "服务器初始化失败\n");
        return 1;
    }
    
    printf("服务器初始化成功\n");
    
    // 启动服务器
    if (!server_start(&config)) {
        fprintf(stderr, "服务器启动失败\n");
        return 1;
    }
    
    printf("服务器启动成功，监听端口 %d\n", config.port);
    printf("按 Ctrl+C 停止服务器\n");
    
    // 运行服务器
    server_run();
    
    printf("服务器已停止\n");
    return 0;
}