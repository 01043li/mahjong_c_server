/**
 * 游戏逻辑头文件
 */
#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "common/tile.h"
#include <stdbool.h>

// 最大玩家数
#define MAX_PLAYERS 4
#define MAX_DISCARD_TILES 100

// 游戏状态
typedef enum {
    GAME_STATUS_WAITING = 0,  // 等待玩家
    GAME_STATUS_PLAYING,      // 游戏中
    GAME_STATUS_FINISHED      // 已结束
} GameStatus;

// 风
typedef enum {
    WIND_EAST = 0,
    WIND_SOUTH,
    WIND_WEST,
    WIND_NORTH
} Wind;

// 玩家结构
typedef struct Player {
    char id[64];              // 玩家ID
    char name[32];            // 玩家名称
    int position;             // 位置 (0-3)
    int score;                // 分数
    Hand* hand;               // 手牌
    Tile discarded[MAX_DISCARD_TILES];  // 弃牌
    int discard_count;        // 弃牌数量
} Player;

// 游戏结构
typedef struct Game {
    int room_id;              // 房间ID
    char room_name[64];       // 房间名称
    Player* players[MAX_PLAYERS];  // 玩家列表
    int player_count;         // 玩家数量
    int current_player;       // 当前玩家索引
    GameStatus status;        // 游戏状态
    Wind wind;                // 当前圈风
    int round;                // 当前局数
    TileSet* tile_set;        // 牌组
} Game;

// 玩家管理
Player* player_create(const char* id, const char* name);
void player_destroy(Player* player);
void player_print(const Player* player);

// 游戏管理
Game* game_create(int room_id, const char* room_name);
void game_destroy(Game* game);
bool game_add_player(Game* game, Player* player);
bool game_remove_player(Game* game, const char* player_id);
bool game_start(Game* game);

// 游戏操作
bool game_play_tile(Game* game, const char* player_id, Tile tile);
Tile game_draw_tile(Game* game, const char* player_id);
bool game_can_win(const Hand* hand, Tile new_tile);

// 状态查询
void game_get_state(const Game* game, char* buffer, size_t buffer_size);
const char* game_status_to_string(GameStatus status);
const char* wind_to_string(Wind wind);

#endif // GAME_LOGIC_H