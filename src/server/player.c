/**
 * 玩家管理模块
 */
#include "server/game_logic.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 创建玩家
Player* player_create(const char* id, const char* name) {
    if (!id || !name) return NULL;
    
    Player* player = (Player*)malloc(sizeof(Player));
    if (!player) return NULL;
    
    // 复制ID和名称
    strncpy(player->id, id, sizeof(player->id) - 1);
    player->id[sizeof(player->id) - 1] = '\0';
    
    strncpy(player->name, name, sizeof(player->name) - 1);
    player->name[sizeof(player->name) - 1] = '\0';
    
    // 初始化其他字段
    player->position = -1;  // 未分配位置
    player->score = 0;
    player->discard_count = 0;
    
    // 创建手牌
    player->hand = hand_create(20);
    if (!player->hand) {
        free(player);
        return NULL;
    }
    
    return player;
}

// 销毁玩家
void player_destroy(Player* player) {
    if (!player) return;
    
    if (player->hand) {
        hand_destroy(player->hand);
    }
    
    free(player);
}

// 打印玩家信息
void player_print(const Player* player) {
    if (!player) {
        printf("玩家: 空\n");
        return;
    }
    
    printf("玩家: %s (%s)\n", player->name, player->id);
    printf("  位置: %d\n", player->position);
    printf("  分数: %d\n", player->score);
    
    // 打印手牌
    if (player->hand && player->hand->count > 0) {
        printf("  手牌(%d张): ", player->hand->count);
        for (int i = 0; i < player->hand->count; i++) {
            printf("%s ", tile_to_string(&player->hand->tiles[i]));
        }
        printf("\n");
    } else {
        printf("  手牌: 空\n");
    }
    
    // 打印弃牌
    if (player->discard_count > 0) {
        printf("  弃牌(%d张): ", player->discard_count);
        for (int i = 0; i < player->discard_count; i++) {
            printf("%s ", tile_to_string(&player->discarded[i]));
        }
        printf("\n");
    }
}