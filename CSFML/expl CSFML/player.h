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
typedef struct {
    sfText* text;
    sfVector2f position;
    float timer; // Durée pendant laquelle le texte est visible
    int alpha;   // Opacité actuelle (0-255)
} bonus_animation_t;

typedef struct bubble_animation {
    bubble_t* bubble;      // La bulle associée à l'animation
    float scale;           // Échelle actuelle de la bulle
    int alpha;             // Transparence actuelle (0-255)
    float timer;           // Durée restante de l'animation
    struct bubble_animation* next; // Liste chaînée pour gérer plusieurs animations
} bubble_animation_t;



typedef struct player {
    bubble_t* grid[ROWS][COLS];
    bubble_t* current;
    bubble_t* next_bubble; // Ajout pour stocker la prochaine bulle
	bubble_t* falling_bubbles;
    float angle;
    int score;
    int defeat;
    sfVector2f launcher_pos;
    bonus_animation_t* bonus_animation;
    bubble_animation_t* animations; // Liste des animations en cours

} player_t;

player_t create_player(sfVector2f pos);
void update_player(player_t* player, sfEvent event, int left, int right, int shoot);
void draw_player(player_t* player, sfRenderWindow* window);
void update_bubbles(player_t* player, player_t* player2, float* chrono1, float* chrono2);
void add_random_bubble_line(player_t* player);
void update_falling_bubbles(player_t* player);
void update_bonus_animation(player_t* player, sfRenderWindow* window);
int flood_fill(bubble_t* grid[ROWS][COLS], int row, int col, int color, int visited[ROWS][COLS], bubble_t** cluster, int* count);

void draw_bubble_animations(player_t* player, sfRenderWindow* window);
void update_bubble_animations(player_t* player, float deltaTime);
