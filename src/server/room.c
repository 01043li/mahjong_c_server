/**
 * 房间管理模块
 */
#include "server/game_logic.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 房间管理器
typedef struct RoomManager {
    Game** rooms;           // 房间数组
    int room_count;         // 房间数量
    int capacity;           // 数组容量
} RoomManager;

static RoomManager room_manager = {0};

// 初始化房间管理器
bool room_manager_init(int initial_capacity) {
    room_manager.rooms = (Game**)malloc(sizeof(Game*) * initial_capacity);
    if (!room_manager.rooms) return false;
    
    room_manager.capacity = initial_capacity;
    room_manager.room_count = 0;
    
    for (int i = 0; i < initial_capacity; i++) {
        room_manager.rooms[i] = NULL;
    }
    
    return true;
}

// 销毁房间管理器
void room_manager_destroy(void) {
    for (int i = 0; i < room_manager.room_count; i++) {
        if (room_manager.rooms[i]) {
            game_destroy(room_manager.rooms[i]);
        }
    }
    
    free(room_manager.rooms);
    room_manager.rooms = NULL;
    room_manager.room_count = 0;
    room_manager.capacity = 0;
}

// 扩展房间数组
static bool expand_room_array(void) {
    int new_capacity = room_manager.capacity * 2;
    Game** new_rooms = (Game**)realloc(room_manager.rooms, sizeof(Game*) * new_capacity);
    
    if (!new_rooms) return false;
    
    room_manager.rooms = new_rooms;
    
    // 初始化新空间
    for (int i = room_manager.capacity; i < new_capacity; i++) {
        room_manager.rooms[i] = NULL;
    }
    
    room_manager.capacity = new_capacity;
    return true;
}

// 创建房间
Game* room_create(const char* room_name) {
    if (!room_name || strlen(room_name) == 0) {
        return NULL;
    }
    
    // 检查是否需要扩展
    if (room_manager.room_count >= room_manager.capacity) {
        if (!expand_room_array()) {
            return NULL;
        }
    }
    
    // 生成房间ID
    int room_id = room_manager.room_count + 1;
    
    // 创建游戏
    Game* game = game_create(room_id, room_name);
    if (!game) {
        return NULL;
    }
    
    // 添加到管理器
    room_manager.rooms[room_manager.room_count] = game;
    room_manager.room_count++;
    
    printf("房间创建成功: %s (ID: %d)\n", room_name, room_id);
    
    return game;
}

// 根据ID查找房间
Game* room_find_by_id(int room_id) {
    for (int i = 0; i < room_manager.room_count; i++) {
        if (room_manager.rooms[i] && room_manager.rooms[i]->room_id == room_id) {
            return room_manager.rooms[i];
        }
    }
    
    return NULL;
}

// 根据名称查找房间
Game* room_find_by_name(const char* room_name) {
    if (!room_name) return NULL;
    
    for (int i = 0; i < room_manager.room_count; i++) {
        if (room_manager.rooms[i] && 
            strcmp(room_manager.rooms[i]->room_name, room_name) == 0) {
            return room_manager.rooms[i];
        }
    }
    
    return NULL;
}

// 销毁房间
bool room_destroy(int room_id) {
    for (int i = 0; i < room_manager.room_count; i++) {
        if (room_manager.rooms[i] && room_manager.rooms[i]->room_id == room_id) {
            game_destroy(room_manager.rooms[i]);
            
            // 将后面的房间向前移动
            for (int j = i; j < room_manager.room_count - 1; j++) {
                room_manager.rooms[j] = room_manager.rooms[j + 1];
            }
            
            room_manager.rooms[room_manager.room_count - 1] = NULL;
            room_manager.room_count--;
            
            printf("房间 %d 已销毁\n", room_id);
            return true;
        }
    }
    
    return false;
}

// 列出所有房间
void room_list_all(void) {
    printf("=== 房间列表 (%d个) ===\n", room_manager.room_count);
    
    if (room_manager.room_count == 0) {
        printf("暂无房间\n");
        return;
    }
    
    for (int i = 0; i < room_manager.room_count; i++) {
        Game* game = room_manager.rooms[i];
        if (game) {
            printf("[%d] %s - %s (%d/4玩家)\n",
                   game->room_id,
                   game->room_name,
                   game_status_to_string(game->status),
                   game->player_count);
        }
    }
}

// 获取房间数量
int room_get_count(void) {
    return room_manager.room_count;
}

// 测试函数
void room_test(void) {
    printf("=== 房间管理测试 ===\n");
    
    // 初始化
    if (!room_manager_init(10)) {
        printf("房间管理器初始化失败\n");
        return;
    }
    
    // 创建几个房间
    Game* room1 = room_create("亲友房");
    Game* room2 = room_create("高手房");
    Game* room3 = room_create("新手练习房");
    
    // 列出房间
    room_list_all();
    
    // 查找房间
    Game* found = room_find_by_name("高手房");
    if (found) {
        printf("找到房间: %s (ID: %d)\n", found->room_name, found->room_id);
    }
    
    // 销毁一个房间
    room_destroy(2);
    
    // 再次列出
    room_list_all();
    
    // 清理
    room_manager_destroy();
    
    printf("房间管理测试完成\n");
}