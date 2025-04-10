#pragma once
#include "tools.h"
#include "bubble.h"

#define ROWS 29
#define COLS 19
// constantes pour la grille « hexagonale »
#define H_SPACING 32.0f      // espacement horizontal
#define V_SPACING 28.0f      // espacement vertical
#define BUBBLE_RADIUS 16.0f  // rayon de la bulle

// Par exemple, définir le nombre de lignes pré-remplies
#define INIT_ROWS 5


typedef struct player {
    bubble_t* grid[ROWS][COLS];
    bubble_t* current;
    bubble_t* next_bubble; // Ajout pour stocker la prochaine bulle
	bubble_t* falling_bubbles;
    float angle;
    int score;
    int defeat;
    sfVector2f launcher_pos;
} player_t;

player_t create_player(sfVector2f pos);
void update_player(player_t* player, sfEvent event, int left, int right, int shoot);
void draw_player(player_t* player, sfRenderWindow* window);
void update_bubbles(player_t* player, float* chrono);
void add_random_bubble_line(player_t* player);
void update_falling_bubbles(player_t* player);

