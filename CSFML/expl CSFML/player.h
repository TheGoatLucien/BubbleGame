#pragma once
#include "tools.h"
#include "bubble.h" // Add this include to resolve the bubble_t type

#define ROWS 29
#define COLS 8

typedef struct player {
    bubble_t* grid[ROWS][COLS];
    bubble_t* current;
    float angle;
    int score;
    int defeat;
    sfVector2f launcher_pos;
} player_t;

player_t create_player(sfVector2f pos);
void update_player(player_t* player, sfEvent event, int left, int right, int shoot);
void draw_player(player_t* player, sfRenderWindow* window);
void update_bubbles(player_t* player);