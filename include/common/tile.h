/**
 * 麻将牌型定义头文件
 */
#ifndef TILE_H
#define TILE_H

#include <stdint.h>
#include <stdbool.h>

// 花色枚举
typedef enum {
    SUIT_CHARACTERS = 0,  // 万
    SUIT_BAMBOO,          // 条
    SUIT_DOTS,            // 筒
    SUIT_HONORS,          // 字牌
    SUIT_FLOWERS          // 花牌
} TileSuit;

// 麻将牌结构
typedef struct {
    TileSuit suit;    // 花色
    uint8_t value;    // 数值（1-9或字牌类型）
    bool is_flower;   // 是否是花牌
} Tile;

// 手牌结构
typedef struct {
    Tile* tiles;      // 手牌数组
    int count;        // 手牌数量
    int capacity;     // 数组容量
} Hand;

// 函数声明

// 牌操作
Tile tile_create(TileSuit suit, uint8_t value, bool is_flower);
const char* tile_to_string(const Tile* tile);
bool tile_equal(const Tile* a, const Tile* b);

// 手牌操作
Hand* hand_create(int initial_capacity);
void hand_destroy(Hand* hand);
bool hand_add_tile(Hand* hand, Tile tile);
bool hand_remove_tile(Hand* hand, Tile tile);
void hand_sort(Hand* hand);
int hand_find_tile(const Hand* hand, Tile tile);
void hand_print(const Hand* hand);

// 牌组操作
typedef struct {
    Tile* tiles;      // 所有牌
    int total_count;  // 总牌数
    int remaining;    // 剩余牌数
} TileSet;

TileSet* tileset_create(bool include_flowers);
void tileset_destroy(TileSet* set);
void tileset_shuffle(TileSet* set);
Tile tileset_draw(TileSet* set);
void tileset_draw_multiple(TileSet* set, Tile* output, int count);
int tileset_remaining(const TileSet* set);
void tileset_reset(TileSet* set, bool include_flowers);

#endif // TILE_H