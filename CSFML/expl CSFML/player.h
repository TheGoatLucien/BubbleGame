#pragma once
#include "tools.h"
#include "bubble.h"

#define ROWS 29
#define COLS 8

typedef struct player {
    bubble_t* grid[ROWS][COLS];
    bubble_t* current;
    bubble_t* next_bubble;
    float angle;
    int score;
    int defeat;
    sfVector2f launcher_pos;
    int combo_count; // Ajout pour le système de combo
} player_t;

player_t create_player(sfVector2f pos);
void update_player(player_t* player, sfEvent event, int left, int right, int shoot);
void draw_player(player_t* player, sfRenderWindow* window);
void update_bubbles(player_t* player);
void check_initial_matches(player_t* player);
void detect_floating_bubbles(player_t* player); // Nouvelle fonction pour détecter les bulles flottantes
void snap_to_grid(player_t* player, bubble_t* bubble, float grid_origin_x); // Nouvelle fonction pour le snap précis