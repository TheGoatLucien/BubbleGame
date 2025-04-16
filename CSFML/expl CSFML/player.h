#pragma once
#include "tools.h"
#include "bubble.h"

#define ROWS 29
#define COLS 19
// D�finition des constantes pour la grille hexagonale
#define H_SPACING 32.0f      // Espacement horizontal entre les bulles
#define V_SPACING 28.0f      // Espacement vertical entre les bulles
#define BUBBLE_RADIUS 16.0f  // Rayon d'une bulle

#define INIT_ROWS 5 // Nombre de lignes pr�-remplies au d�but du jeu

// Structure pour les animations de bonus (affichage "+10s", etc.)
typedef struct {
    sfText* text;
    sfVector2f position;
    float timer; // Dur�e pendant laquelle le texte est visible
    int alpha;   // Opacit� actuelle (0-255)
} bonus_animation_t;

// Structure pour les animations de bulles (effets visuels lors des collisions ou des destructions)
typedef struct bubble_animation {
    bubble_t* bubble;      // La bulle associ�e � l'animation
    float scale;           // �chelle actuelle de la bulle (effet de zoom)
    int alpha;             // Transparence actuelle (0-255)
    float timer;           // Dur�e restante de l'animation
    struct bubble_animation* next; // Liste cha�n�e pour g�rer plusieurs animations
} bubble_animation_t;

// Structure principale pour g�rer un joueur
typedef struct player {
    bubble_t* grid[ROWS][COLS]; // Grille contenant les bulles du joueur
    bubble_t* current;          // Bulle actuellement en tir
    bubble_t* next_bubble;      // Prochaine bulle qui sera tir�e
    bubble_t* falling_bubbles;  // Liste des bulles en chute
    float angle;                // Angle de tir du joueur
    int score;                  // Score du joueur
    int defeat;                 // Indicateur de d�faite (1 si perdu)
    sfVector2f launcher_pos;    // Position du lanceur
    bonus_animation_t* bonus_animation; // Animation de bonus en cours
    bubble_animation_t* animations;     // Liste des animations de bulles

} player_t;

// Fonction pour cr�er un joueur et initialiser sa grille
player_t create_player(sfVector2f pos);

// Mise � jour des mouvements du joueur (direction du tir)
void update_player(player_t* player, sfEvent event, int left, int right, int shoot);

// Dessin des �l�ments du joueur
void draw_player(player_t* player, sfRenderWindow* window);

// Gestion des mises � jour des bulles (attache apr�s tir, suppression apr�s match)
void update_bubbles(player_t* player, player_t* player2, float* chrono1, float* chrono2);

// Ajout d'une ligne de bulles al�atoires dans la grille
void add_random_bubble_line(player_t* player);

// Mise � jour des bulles en chute libre
void update_falling_bubbles(player_t* player);

// Mise � jour des animations de bonus (affichage du texte "+10s")
void update_bonus_animation(player_t* player, sfRenderWindow* window);

// Algorithme de flood-fill pour d�tecter les groupes de bulles connect�es
int flood_fill(bubble_t* grid[ROWS][COLS], int row, int col, int color, int visited[ROWS][COLS], bubble_t** cluster, int* count);

// Dessin des animations de bulles lors des destructions ou effets sp�ciaux
void draw_bubble_animations(player_t* player, sfRenderWindow* window);

// Mise � jour des animations de bulles
void update_bubble_animations(player_t* player, float deltaTime);

