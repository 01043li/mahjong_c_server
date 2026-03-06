/**
 * 麻将牌测试
 */
#include "common/tile.h"
#include <stdio.h>
#include <assert.h>

void test_tile_creation(void) {
    printf("测试牌创建...\n");
    
    // 测试创建各种牌
    Tile tile1 = tile_create(SUIT_CHARACTERS, 5, false);
    Tile tile2 = tile_create(SUIT_BAMBOO, 3, false);
    Tile tile3 = tile_create(SUIT_HONORS, 1, false);  // 东
    
    assert(tile1.suit == SUIT_CHARACTERS);
    assert(tile1.value == 5);
    assert(!tile1.is_flower);
    
    assert(tile2.suit == SUIT_BAMBOO);
    assert(tile2.value == 3);
    
    assert(tile3.suit == SUIT_HONORS);
    assert(tile3.value == 1);
    
    printf("  牌创建测试通过\n");
}

void test_tile_to_string(void) {
    printf("测试牌转字符串...\n");
    
    Tile tile1 = tile_create(SUIT_CHARACTERS, 5, false);
    Tile tile2 = tile_create(SUIT_HONORS, 1, false);  // 东
    Tile tile3 = tile_create(SUIT_FLOWERS, 3, true);  // 花3
    
    const char* str1 = tile_to_string(&tile1);
    const char* str2 = tile_to_string(&tile2);
    const char* str3 = tile_to_string(&tile3);
    
    printf("  %s -> %s\n", str1, "5万");
    printf("  %s -> %s\n", str2, "东");
    printf("  %s -> %s\n", str3, "花3");
    
    printf("  牌转字符串测试通过\n");
}

void test_tile_equal(void) {
    printf("测试牌相等比较...\n");
    
    Tile tile1 = tile_create(SUIT_CHARACTERS, 5, false);
    Tile tile2 = tile_create(SUIT_CHARACTERS, 5, false);
    Tile tile3 = tile_create(SUIT_CHARACTERS, 6, false);
    Tile tile4 = tile_create(SUIT_BAMBOO, 5, false);
    
    assert(tile_equal(&tile1, &tile2));
    assert(!tile_equal(&tile1, &tile3));
    assert(!tile_equal(&tile1, &tile4));
    
    printf("  牌相等比较测试通过\n");
}

void test_hand_operations(void) {
    printf("测试手牌操作...\n");
    
    // 创建手牌
    Hand* hand = hand_create(10);
    assert(hand != NULL);
    assert(hand->count == 0);
    
    // 添加牌
    Tile tile1 = tile_create(SUIT_CHARACTERS, 5, false);
    Tile tile2 = tile_create(SUIT_BAMBOO, 3, false);
    Tile tile3 = tile_create(SUIT_DOTS, 7, false);
    
    assert(hand_add_tile(hand, tile1));
    assert(hand_add_tile(hand, tile2));
    assert(hand_add_tile(hand, tile3));
    assert(hand->count == 3);
    
    // 查找牌
    assert(hand_find_tile(hand, tile1) >= 0);
    assert(hand_find_tile(hand, tile2) >= 0);
    
    // 移除牌
    assert(hand_remove_tile(hand, tile2));
    assert(hand->count == 2);
    assert(hand_find_tile(hand, tile2) == -1);
    
    // 排序
    hand_sort(hand);
    
    // 打印手牌
    printf("  手牌: ");
    hand_print(hand);
    
    // 清理
    hand_destroy(hand);
    
    printf("  手牌操作测试通过\n");
}

void test_tileset_operations(void) {
    printf("测试牌组操作...\n");
    
    // 创建牌组（不含花牌）
    TileSet* set = tileset_create(false);
    assert(set != NULL);
    assert(set->total_count == 136);  // 标准麻将136张
    
    // 洗牌
    tileset_shuffle(set);
    assert(set->remaining == 136);
    
    // 摸牌
    Tile tile1 = tileset_draw(set);
    Tile tile2 = tileset_draw(set);
    assert(set->remaining == 134);
    
    printf("  摸到牌: %s, %s\n", tile_to_string(&tile1), tile_to_string(&tile2));
    
    // 摸多张牌
    Tile tiles[10];
    tileset_draw_multiple(set, tiles, 10);
    assert(set->remaining == 124);
    
    // 创建手牌并添加摸到的牌
    Hand* hand = hand_create(20);
    for (int i = 0; i < 10; i++) {
        hand_add_tile(hand, tiles[i]);
    }
    
    // 排序并显示
    hand_sort(hand);
    printf("  10张手牌: ");
    for (int i = 0; i < hand->count; i++) {
        printf("%s ", tile_to_string(&hand->tiles[i]));
    }
    printf("\n");
    
    // 清理
    hand_destroy(hand);
    tileset_destroy(set);
    
    printf("  牌组操作测试通过\n");
}

void test_edge_cases(void) {
    printf("测试边界情况...\n");
    
    // 测试空手牌
    Hand* hand = hand_create(0);
    assert(hand != NULL);
    hand_destroy(hand);
    
    // 测试空牌组
    TileSet* set = tileset_create(false);
    assert(set != NULL);
    
    // 摸空牌组
    for (int i = 0; i < 136; i++) {
        Tile tile = tileset_draw(set);
        assert(tile.suit != SUIT_CHARACTERS || tile.value != 0);  // 不应该返回无效牌
    }
    
    // 牌组已空
    Tile tile = tileset_draw(set);
    assert(tile.suit == SUIT_CHARACTERS && tile.value == 0);  // 应该返回无效牌
    
    tileset_destroy(set);
    
    printf("  边界情况测试通过\n");
}

int main(void) {
    printf("=== 开始麻将牌测试 ===\n\n");
    
    test_tile_creation();
    test_tile_to_string();
    test_tile_equal();
    test_hand_operations();
    test_tileset_operations();
    test_edge_cases();
    
    printf("\n=== 所有测试通过 ===\n");
    return 0;
}