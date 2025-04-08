#pragma once
#include "tools.h"
#include "bubble.h"

#define ROWS 29
#define COLS 8
// Constantes pour la grille hexagonale (quinconce)
#define H_SPACING 32.0f      
#define V_SPACING 28.0f      
#define BUBBLE_RADIUS 16.0f  
#define INIT_ROWS 5          

// Position du plafond (définit la zone d'attache en haut)
#define CEILING_Y 60.0f

typedef struct player {
    bubble_t* grid[ROWS][COLS];
    bubble_t* current;
    bubble_t* next_bubble;     // Bulle à tirer
    bubble_t* falling_bubbles; // Liste des bulles détachées (affichage de chute)
    float angle;
    int score;
    int defeat;
    sfVector2f launcher_pos;
} player_t;

player_t create_player(sfVector2f pos);
void update_player(player_t* player, sfEvent event, int left, int right, int shoot);
void draw_player(player_t* player, sfRenderWindow* window);
void update_bubbles(player_t* player);
void drop_floating_bubbles(player_t* player);
void update_falling_bubbles(player_t* player);
int flood_fill(bubble_t* grid[ROWS][COLS], int row, int col, int color, int visited[ROWS][COLS], bubble_t** cluster, int* count);
