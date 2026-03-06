/**
 * 游戏逻辑模块
 */
#include "server/game_logic.h"
#include "common/tile.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 创建游戏
Game* game_create(int room_id, const char* room_name) {
    Game* game = (Game*)malloc(sizeof(Game));
    if (!game) return NULL;
    
    game->room_id = room_id;
    strncpy(game->room_name, room_name, sizeof(game->room_name) - 1);
    game->room_name[sizeof(game->room_name) - 1] = '\0';
    
    game->player_count = 0;
    game->current_player = 0;
    game->status = GAME_STATUS_WAITING;
    game->wind = WIND_EAST;
    game->round = 1;
    
    // 初始化玩家
    for (int i = 0; i < MAX_PLAYERS; i++) {
        game->players[i] = NULL;
    }
    
    // 创建牌组
    game->tile_set = tileset_create(false);
    
    return game;
}

// 销毁游戏
void game_destroy(Game* game) {
    if (!game) return;
    
    // 销毁玩家
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (game->players[i]) {
            player_destroy(game->players[i]);
        }
    }
    
    // 销毁牌组
    if (game->tile_set) {
        tileset_destroy(game->tile_set);
    }
    
    free(game);
}

// 添加玩家到游戏
bool game_add_player(Game* game, Player* player) {
    if (!game || !player || game->player_count >= MAX_PLAYERS) {
        return false;
    }
    
    // 查找空位
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (game->players[i] == NULL) {
            game->players[i] = player;
            game->player_count++;
            
            // 设置玩家位置
            player->position = i;
            
            return true;
        }
    }
    
    return false;
}

// 从游戏中移除玩家
bool game_remove_player(Game* game, const char* player_id) {
    if (!game || !player_id) return false;
    
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (game->players[i] && strcmp(game->players[i]->id, player_id) == 0) {
            player_destroy(game->players[i]);
            game->players[i] = NULL;
            game->player_count--;
            return true;
        }
    }
    
    return false;
}

// 开始游戏
bool game_start(Game* game) {
    if (!game || game->player_count < 2 || game->status != GAME_STATUS_WAITING) {
        return false;
    }
    
    // 洗牌
    tileset_shuffle(game->tile_set);
    
    // 给每个玩家发13张牌
    for (int i = 0; i < game->player_count; i++) {
        if (game->players[i]) {
            Tile tiles[13];
            tileset_draw_multiple(game->tile_set, tiles, 13);
            
            for (int j = 0; j < 13; j++) {
                hand_add_tile(game->players[i]->hand, tiles[j]);
            }
            
            hand_sort(game->players[i]->hand);
        }
    }
    
    // 额外给庄家一张牌
    if (game->players[game->current_player]) {
        Tile tile = tileset_draw(game->tile_set);
        hand_add_tile(game->players[game->current_player]->hand, tile);
        hand_sort(game->players[game->current_player]->hand);
    }
    
    game->status = GAME_STATUS_PLAYING;
    
    return true;
}

// 玩家打牌
bool game_play_tile(Game* game, const char* player_id, Tile tile) {
    if (!game || !player_id || game->status != GAME_STATUS_PLAYING) {
        return false;
    }
    
    // 找到玩家
    Player* player = NULL;
    int player_index = -1;
    
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (game->players[i] && strcmp(game->players[i]->id, player_id) == 0) {
            player = game->players[i];
            player_index = i;
            break;
        }
    }
    
    if (!player || player_index != game->current_player) {
        return false;  // 不是当前玩家的回合
    }
    
    // 从手牌中移除这张牌
    if (!hand_remove_tile(player->hand, tile)) {
        return false;  // 玩家没有这张牌
    }
    
    // 添加到弃牌堆
    player->discard_count++;
    if (player->discard_count < MAX_DISCARD_TILES) {
        player->discarded[player->discard_count - 1] = tile;
    }
    
    // 切换到下一个玩家
    game->current_player = (game->current_player + 1) % game->player_count;
    
    return true;
}

// 玩家摸牌
Tile game_draw_tile(Game* game, const char* player_id) {
    if (!game || !player_id || game->status != GAME_STATUS_PLAYING) {
        return tile_create(SUIT_CHARACTERS, 0, false);  // 返回无效牌
    }
    
    // 检查是否是当前玩家
    Player* player = NULL;
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (game->players[i] && strcmp(game->players[i]->id, player_id) == 0) {
            if (i != game->current_player) {
                return tile_create(SUIT_CHARACTERS, 0, false);
            }
            player = game->players[i];
            break;
        }
    }
    
    if (!player) {
        return tile_create(SUIT_CHARACTERS, 0, false);
    }
    
    // 摸牌
    Tile tile = tileset_draw(game->tile_set);
    hand_add_tile(player->hand, tile);
    hand_sort(player->hand);
    
    return tile;
}

// 检查是否可以胡牌（简化版）
bool game_can_win(const Hand* hand, Tile new_tile) {
    if (!hand) return false;
    
    // 这里实现胡牌判断逻辑
    // 简化版：暂时返回false
    return false;
}

// 获取游戏状态
void game_get_state(const Game* game, char* buffer, size_t buffer_size) {
    if (!game || !buffer) return;
    
    snprintf(buffer, buffer_size,
             "房间: %s (ID: %d)\n"
             "状态: %s\n"
             "玩家: %d/4\n"
             "当前玩家: %d\n"
             "圈风: %s\n"
             "剩余牌数: %d\n",
             game->room_name,
             game->room_id,
             game_status_to_string(game->status),
             game->player_count,
             game->current_player,
             wind_to_string(game->wind),
             tileset_remaining(game->tile_set));
}

// 状态转字符串
const char* game_status_to_string(GameStatus status) {
    switch (status) {
        case GAME_STATUS_WAITING: return "等待中";
        case GAME_STATUS_PLAYING: return "游戏中";
        case GAME_STATUS_FINISHED: return "已结束";
        default: return "未知";
    }
}

// 风转字符串
const char* wind_to_string(Wind wind) {
    switch (wind) {
        case WIND_EAST: return "东";
        case WIND_SOUTH: return "南";
        case WIND_WEST: return "西";
        case WIND_NORTH: return "北";
        default: return "未知";
    }
}