#pragma once
#include "tools.h"
#include "bubble.h"

#define ROWS 29
#define COLS 8

#define H_SPACING 32.0f
#define V_SPACING 28.0f
#define BUBBLE_RADIUS 16.0f
#define INIT_ROWS 5
#define CEILING_Y 100.0f  // Décalage pour laisser place au score

typedef struct player {
    bubble_t* grid[ROWS][COLS];
    bubble_t* current;
    bubble_t* next_bubble;
    bubble_t* falling_bubbles;
    float angle;
    int score;
    int defeat;
    sfVector2f launcher_pos;
} player_t;

// Fonctions principales
player_t create_player(sfVector2f pos);
void update_player(player_t* player, sfEvent event, int left, int right, int shoot);
void update_bubbles(player_t* player);
void update_falling_bubbles(player_t* player);
void drop_floating_bubbles(player_t* player);
void draw_player(player_t* player, sfRenderWindow* window);

// Flood fill pour trouver grappes
int flood_fill(bubble_t* grid[ROWS][COLS], int row, int col, int color,
    int visited[ROWS][COLS], bubble_t** cluster, int* count);
