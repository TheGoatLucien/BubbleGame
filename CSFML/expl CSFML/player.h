#pragma once
#include "tools.h"
#include "bubble.h"

#define ROWS 29
#define COLS 19
// Définition des constantes pour la grille hexagonale
#define H_SPACING 32.0f      // Espacement horizontal entre les bulles
#define V_SPACING 28.0f      // Espacement vertical entre les bulles
#define BUBBLE_RADIUS 16.0f  // Rayon d'une bulle

#define INIT_ROWS 5 // Nombre de lignes pré-remplies au début du jeu

// Structure pour les animations de bonus (affichage "+10s", etc.)
typedef struct {
    sfText* text;
    sfVector2f position;
    float timer; // Durée pendant laquelle le texte est visible
    int alpha;   // Opacité actuelle (0-255)
} bonus_animation_t;

// Structure pour les animations de bulles (effets visuels lors des collisions ou des destructions)
typedef struct bubble_animation {
    bubble_t* bubble;      // La bulle associée à l'animation
    float scale;           // Échelle actuelle de la bulle (effet de zoom)
    int alpha;             // Transparence actuelle (0-255)
    float timer;           // Durée restante de l'animation
    struct bubble_animation* next; // Liste chaînée pour gérer plusieurs animations
} bubble_animation_t;

// Structure principale pour gérer un joueur
typedef struct player {
    bubble_t* grid[ROWS][COLS]; // Grille contenant les bulles du joueur
    bubble_t* current;          // Bulle actuellement en tir
    bubble_t* next_bubble;      // Prochaine bulle qui sera tirée
    bubble_t* falling_bubbles;  // Liste des bulles en chute
    float angle;                // Angle de tir du joueur
    int score;                  // Score du joueur
    int defeat;                 // Indicateur de défaite (1 si perdu)
    sfVector2f launcher_pos;    // Position du lanceur
    bonus_animation_t* bonus_animation; // Animation de bonus en cours
    bubble_animation_t* animations;     // Liste des animations de bulles

} player_t;

// Fonction pour créer un joueur et initialiser sa grille
player_t create_player(sfVector2f pos);

// Mise à jour des mouvements du joueur (direction du tir)
void update_player(player_t* player, sfEvent event, int left, int right, int shoot);

// Dessin des éléments du joueur
void draw_player(player_t* player, sfRenderWindow* window);

// Gestion des mises à jour des bulles (attache après tir, suppression après match)
void update_bubbles(player_t* player, player_t* player2, float* chrono1, float* chrono2);

// Ajout d'une ligne de bulles aléatoires dans la grille
void add_random_bubble_line(player_t* player);

// Mise à jour des bulles en chute libre
void update_falling_bubbles(player_t* player);

// Mise à jour des animations de bonus (affichage du texte "+10s")
void update_bonus_animation(player_t* player, sfRenderWindow* window);

// Algorithme de flood-fill pour détecter les groupes de bulles connectées
int flood_fill(bubble_t* grid[ROWS][COLS], int row, int col, int color, int visited[ROWS][COLS], bubble_t** cluster, int* count);

// Dessin des animations de bulles lors des destructions ou effets spéciaux
void draw_bubble_animations(player_t* player, sfRenderWindow* window);

// Mise à jour des animations de bulles
void update_bubble_animations(player_t* player, float deltaTime);

