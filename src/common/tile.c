/**
 * 麻将牌型实现
 */
#include "common/tile.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// 花色名称
static const char* suit_names[] = {
    "万", "条", "筒", "字", "花"
};

// 字牌名称
static const char* honor_names[] = {
    "东", "南", "西", "北", "中", "发", "白"
};

// 创建一张牌
Tile tile_create(TileSuit suit, uint8_t value, bool is_flower) {
    Tile tile;
    tile.suit = suit;
    tile.value = value;
    tile.is_flower = is_flower;
    return tile;
}

// 将牌转换为字符串
const char* tile_to_string(const Tile* tile) {
    static char buffer[32];
    
    if (tile->suit == SUIT_HONORS) {
        if (tile->value >= 1 && tile->value <= 7) {
            snprintf(buffer, sizeof(buffer), "%s", honor_names[tile->value - 1]);
        } else {
            snprintf(buffer, sizeof(buffer), "未知字牌");
        }
    } else if (tile->suit == SUIT_FLOWERS) {
        snprintf(buffer, sizeof(buffer), "花%d", tile->value);
    } else {
        snprintf(buffer, sizeof(buffer), "%d%s", tile->value, suit_names[tile->suit]);
    }
    
    return buffer;
}

// 比较两张牌是否相同
bool tile_equal(const Tile* a, const Tile* b) {
    return a->suit == b->suit && 
           a->value == b->value && 
           a->is_flower == b->is_flower;
}

// 创建手牌
Hand* hand_create(int initial_capacity) {
    Hand* hand = (Hand*)malloc(sizeof(Hand));
    if (!hand) return NULL;
    
    hand->capacity = initial_capacity > 0 ? initial_capacity : 20;
    hand->count = 0;
    hand->tiles = (Tile*)malloc(sizeof(Tile) * hand->capacity);
    
    if (!hand->tiles) {
        free(hand);
        return NULL;
    }
    
    return hand;
}

// 销毁手牌
void hand_destroy(Hand* hand) {
    if (hand) {
        free(hand->tiles);
        free(hand);
    }
}

// 扩展手牌容量
static bool hand_expand(Hand* hand) {
    int new_capacity = hand->capacity * 2;
    Tile* new_tiles = (Tile*)realloc(hand->tiles, sizeof(Tile) * new_capacity);
    
    if (!new_tiles) return false;
    
    hand->tiles = new_tiles;
    hand->capacity = new_capacity;
    return true;
}

// 添加一张牌到手牌
bool hand_add_tile(Hand* hand, Tile tile) {
    if (!hand) return false;
    
    // 检查是否需要扩展
    if (hand->count >= hand->capacity) {
        if (!hand_expand(hand)) return false;
    }
    
    hand->tiles[hand->count] = tile;
    hand->count++;
    return true;
}

// 从手牌中移除一张牌
bool hand_remove_tile(Hand* hand, Tile tile) {
    if (!hand) return false;
    
    for (int i = 0; i < hand->count; i++) {
        if (tile_equal(&hand->tiles[i], &tile)) {
            // 将后面的牌向前移动
            for (int j = i; j < hand->count - 1; j++) {
                hand->tiles[j] = hand->tiles[j + 1];
            }
            hand->count--;
            return true;
        }
    }
    
    return false;
}

// 比较两张牌的排序顺序
static int tile_compare(const void* a, const void* b) {
    const Tile* tile_a = (const Tile*)a;
    const Tile* tile_b = (const Tile*)b;
    
    // 先按花色排序
    if (tile_a->suit != tile_b->suit) {
        return tile_a->suit - tile_b->suit;
    }
    
    // 同花色按数值排序
    return tile_a->value - tile_b->value;
}

// 对手牌进行排序
void hand_sort(Hand* hand) {
    if (!hand || hand->count <= 1) return;
    
    qsort(hand->tiles, hand->count, sizeof(Tile), tile_compare);
}

// 在手牌中查找指定的牌
int hand_find_tile(const Hand* hand, Tile tile) {
    if (!hand) return -1;
    
    for (int i = 0; i < hand->count; i++) {
        if (tile_equal(&hand->tiles[i], &tile)) {
            return i;
        }
    }
    
    return -1;
}

// 打印手牌
void hand_print(const Hand* hand) {
    if (!hand) {
        printf("手牌为空\n");
        return;
    }
    
    printf("手牌(%d张): ", hand->count);
    for (int i = 0; i < hand->count; i++) {
        printf("%s ", tile_to_string(&hand->tiles[i]));
    }
    printf("\n");
}

// 创建牌组
TileSet* tileset_create(bool include_flowers) {
    TileSet* set = (TileSet*)malloc(sizeof(TileSet));
    if (!set) return NULL;
    
    // 计算总牌数
    int total = 0;
    
    // 万、条、筒各36张（1-9各4张）
    total += 3 * 9 * 4;  // 3种花色 * 9个数字 * 4张
    
    // 字牌28张（东南西北中发白各4张）
    total += 7 * 4;
    
    // 花牌（可选）
    if (include_flowers) {
        total += 8;  // 8张花牌
    }
    
    set->total_count = total;
    set->remaining = total;
    set->tiles = (Tile*)malloc(sizeof(Tile) * total);
    
    if (!set->tiles) {
        free(set);
        return NULL;
    }
    
    // 初始化牌组
    int index = 0;
    
    // 万、条、筒
    TileSuit suits[] = {SUIT_CHARACTERS, SUIT_BAMBOO, SUIT_DOTS};
    for (int s = 0; s < 3; s++) {
        for (int value = 1; value <= 9; value++) {
            for (int copy = 0; copy < 4; copy++) {
                set->tiles[index++] = tile_create(suits[s], value, false);
            }
        }
    }
    
    // 字牌
    for (int value = 1; value <= 7; value++) {
        for (int copy = 0; copy < 4; copy++) {
            set->tiles[index++] = tile_create(SUIT_HONORS, value, false);
        }
    }
    
    // 花牌
    if (include_flowers) {
        for (int value = 1; value <= 8; value++) {
            set->tiles[index++] = tile_create(SUIT_FLOWERS, value, true);
        }
    }
    
    return set;
}

// 销毁牌组
void tileset_destroy(TileSet* set) {
    if (set) {
        free(set->tiles);
        free(set);
    }
}

// 洗牌
void tileset_shuffle(TileSet* set) {
    if (!set) return;
    
    srand(time(NULL));
    
    // Fisher-Yates洗牌算法
    for (int i = set->total_count - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        
        // 交换
        Tile temp = set->tiles[i];
        set->tiles[i] = set->tiles[j];
        set->tiles[j] = temp;
    }
    
    set->remaining = set->total_count;
}

// 摸一张牌
Tile tileset_draw(TileSet* set) {
    if (!set || set->remaining <= 0) {
        // 返回一张无效的牌
        return tile_create(SUIT_CHARACTERS, 0, false);
    }
    
    Tile tile = set->tiles[set->total_count - set->remaining];
    set->remaining--;
    return tile;
}

// 摸多张牌
void tileset_draw_multiple(TileSet* set, Tile* output, int count) {
    if (!set || !output || count <= 0) return;
    
    for (int i = 0; i < count && set->remaining > 0; i++) {
        output[i] = tileset_draw(set);
    }
}

// 获取剩余牌数
int tileset_remaining(const TileSet* set) {
    return set ? set->remaining : 0;
}

// 重置牌组
void tileset_reset(TileSet* set, bool include_flowers) {
    if (!set) return;
    
    tileset_destroy(set);
    TileSet* new_set = tileset_create(include_flowers);
    if (new_set) {
        *set = *new_set;
        free(new_set);
    }
}