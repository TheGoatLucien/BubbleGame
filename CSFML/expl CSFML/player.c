#include "player.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

player_t create_player(sfVector2f pos) {
    player_t player = { 0 };
    player.launcher_pos = pos;
    player.angle = -3.14f / 2;
    player.score = 0;
    player.defeat = 0;
    player.current = NULL;
    player.next_bubble = create_bubble(pos, 0); // création de la prochaine bulle

    // Calcul de la position d'origine de la grille :
    float gridOriginX = player.launcher_pos.x - (COLS * H_SPACING) / 2;
    float gridOriginY = 16.0f;  // on positionne en haut

    // On initialise les premières lignes en quinconce
    for (int i = 0; i < INIT_ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            bubble_t* b = malloc(sizeof(bubble_t));
            if (!b) continue;
            b->color = rand() % 4 + 1; // couleur jouable entre 1 et 4
            // Pour les lignes impaires, décaler horizontalement de H_SPACING/2 
            float offsetX = (i % 2 == 0) ? 0 : H_SPACING / 2;
            b->pos.x = gridOriginX + offsetX + j * H_SPACING + BUBBLE_RADIUS;
            b->pos.y = gridOriginY + i * V_SPACING;
            b->active = 0;
            b->next = NULL;
            player.grid[i][j] = b;
        }
    }
    return player;
}


void update_player(player_t* player, sfEvent event, int left, int right, int shoot) {
    float delta = getDeltaTime();
    float rotationSpeed = 2.5f;

    if (sfKeyboard_isKeyPressed(left)) player->angle -= rotationSpeed * delta;
    if (sfKeyboard_isKeyPressed(right)) player->angle += rotationSpeed * delta;

    if (player->angle < -2.5f) player->angle = -2.5f;
    if (player->angle > -0.6f) player->angle = -0.6f;

    if (sfKeyboard_isKeyPressed(shoot) && player->current == NULL) {
        player->current = player->next_bubble;
        player->current->active = 1;
        player->next_bubble = create_bubble(player->launcher_pos, 0); // Générer une nouvelle bulle
    }

    if (player->current != NULL) {
        player->current->pos.x += cos(player->angle) * 800 * delta;
        player->current->pos.y += sin(player->angle) * 800 * delta;

        // Rebond mur gauche/droit
        if (player->current->pos.x <= 0 || player->current->pos.x >= WINDOWS_WIDHT) {
            float dx = cos(player->angle);
            float dy = sin(player->angle);
            player->angle = atan2(dy, -dx);
        }

        // Rebond mur central
        float centerX = WINDOWS_WIDHT / 2;
        float margin = 10;

        if ((player->launcher_pos.x < centerX && player->current->pos.x >= centerX - margin) ||
            (player->launcher_pos.x > centerX && player->current->pos.x <= centerX + margin)) {
            float dx = cos(player->angle);
            float dy = sin(player->angle);
            player->angle = atan2(dy, -dx);
            if (player->launcher_pos.x < centerX)
                player->current->pos.x = centerX - margin;
            else
                player->current->pos.x = centerX + margin;
        }

        // Plafond
        if (player->current->pos.y < 0) {
            destroy_bubble(player->current);
            player->current = NULL;
            player->next_bubble = create_bubble(player->launcher_pos, 0); // Nouvelle bulle après destruction
        }
    }
}

void draw_aim_line(sfVector2f origin, float angle, sfRenderWindow* window) {
    sfVertex line[] = {
        { origin, sfColor_fromRGBA(255, 255, 255, 80) },
        { (sfVector2f) {
            origin.x + cos(angle) * 800,
            origin.y + sin(angle) * 800
        }, sfColor_fromRGBA(255, 255, 255, 80) }
    };

    sfRenderWindow_drawPrimitives(window, line, 2, sfLines, NULL);
}

void draw_player(player_t* player, sfRenderWindow* window) {
    sfRectangleShape* midWall = sfRectangleShape_create();
    sfRectangleShape_setSize(midWall, (sfVector2f) { 10, WINDOWS_HEIGHT });
    sfRectangleShape_setFillColor(midWall, sfColor_fromRGBA(255, 255, 255, 60));
    sfRectangleShape_setPosition(midWall, (sfVector2f) { (WINDOWS_WIDHT / 2) - 5, 0 });
    sfRenderWindow_drawRectangleShape(window, midWall, NULL);
    sfRectangleShape_destroy(midWall);

    draw_aim_line(player->launcher_pos, player->angle, window);

    sfCircleShape* launcher = sfCircleShape_create();
    sfCircleShape_setRadius(launcher, 20);
    sfCircleShape_setOrigin(launcher, (sfVector2f) { 20, 20 });
    sfCircleShape_setFillColor(launcher, sfWhite);
    sfCircleShape_setPosition(launcher, player->launcher_pos);
    sfRenderWindow_drawCircleShape(window, launcher, NULL);
    sfCircleShape_destroy(launcher);

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (player->grid[i][j]) {
                draw_bubble(player->grid[i][j], window);
            }
        }
    }

    if (player->current) {
        draw_bubble(player->current, window);
    }
    else {
        sfCircleShape* preview = sfCircleShape_create();
        sfCircleShape_setRadius(preview, 16);
        sfCircleShape_setOrigin(preview, (sfVector2f) { 16, 16 });
        sfCircleShape_setPosition(preview, player->launcher_pos);

        // Utiliser la couleur de la prochaine bulle
        sfColor color;
        switch (player->next_bubble->color) {
        case 0: color = sfColor_fromRGB(128, 128, 128); break;
        case 1: color = sfRed; break;
        case 2: color = sfBlue; break;
        case 3: color = sfGreen; break;
        case 4: color = sfYellow; break;
        default: color = sfWhite; break;
        }
        sfCircleShape_setFillColor(preview, color);
        sfRenderWindow_drawCircleShape(window, preview, NULL);
        sfCircleShape_destroy(preview);
    }
}

int flood_fill(bubble_t* grid[ROWS][COLS], int row, int col, int color, int visited[ROWS][COLS], bubble_t** cluster, int* count) {
    // Vérification des limites et conditions de correspondance
    if (row < 0 || row >= ROWS || col < 0 || col >= COLS)
        return 0;
    if (visited[row][col] || !grid[row][col] || grid[row][col]->color != color)
        return 0;

    visited[row][col] = 1;
    cluster[(*count)++] = grid[row][col];

    // Dépendant de la parité de la ligne, les voisins diffèrent :
    // Pour les lignes paires (index 0, 2, 4, …)
    int offsetsEven[6][2] = {
        {0, -1},   // Gauche
        {0, 1},    // Droite
        {-1, 0},   // Haut
        {-1, -1},  // Haut gauche
        {1, 0},    // Bas
        {1, -1}    // Bas gauche
    };
    // Pour les lignes impaires (index 1, 3, 5, …)
    int offsetsOdd[6][2] = {
        {0, -1},   // Gauche
        {0, 1},    // Droite
        {-1, 0},   // Haut
        {-1, 1},   // Haut droite
        {1, 0},    // Bas
        {1, 1}     // Bas droite
    };

    int (*offsets)[2] = (row % 2 == 0) ? offsetsEven : offsetsOdd;

    for (int i = 0; i < 6; i++) {
        int newRow = row + offsets[i][0];
        int newCol = col + offsets[i][1];
        flood_fill(grid, newRow, newCol, color, visited, cluster, count);
    }

    return *count;
}


void update_bubbles(player_t* player) {
    if (!player->current) return;

    // On vérifie si la bulle frappe le plafond
    if (player->current->pos.y <= 0)
        goto attach_bubble;

    // Vérification de collision sur la grille
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (player->grid[i][j] &&
                Circle_Collision(player->current->pos, player->grid[i][j]->pos, BUBBLE_RADIUS, BUBBLE_RADIUS)) {
                goto attach_bubble;
            }
        }
    }
    return;

attach_bubble:;
    float gridOriginX = player->launcher_pos.x - (COLS * H_SPACING) / 2;

    // Déterminer la ligne d’attache en se basant sur l’ordonnée
    int row = fClamp(player->current->pos.y / V_SPACING, 0, ROWS - 1);
    int col;
    if (row % 2 == 0)
        col = fClamp((player->current->pos.x - gridOriginX) / H_SPACING, 0, COLS - 1);
    else
        col = fClamp((player->current->pos.x - (gridOriginX + H_SPACING / 2)) / H_SPACING, 0, COLS - 1);

    // Position ajustée pour respecter le quinconce
    float offsetX = (row % 2 == 0) ? 0 : H_SPACING / 2;
    player->current->pos.x = gridOriginX + offsetX + col * H_SPACING + BUBBLE_RADIUS;
    player->current->pos.y = row * V_SPACING + BUBBLE_RADIUS;

    if (!player->grid[row][col]) {
        player->grid[row][col] = player->current;
        player->current = NULL;

        // Détection de cluster avec la fonction refaite.
        int visited[ROWS][COLS] = { 0 };
        bubble_t* cluster[ROWS * COLS] = { 0 };
        int count = 0;
        count = flood_fill(player->grid, row, col, player->grid[row][col]->color, visited, cluster, &count);

        if (count >= 3) {
            for (int i = 0; i < count; i++) {
                // On parcourt toute la grille pour détruire les bulles du cluster
                for (int r = 0; r < ROWS; r++) {
                    for (int c = 0; c < COLS; c++) {
                        if (player->grid[r][c] == cluster[i]) {
                            destroy_bubble(player->grid[r][c]);
                            player->grid[r][c] = NULL;
                        }
                    }
                }
            }
            player->score += count;
        }
    }
    else {
        // Si la case est déjà occupée, on supprime la bulle tirée et en regénère une nouvelle
        destroy_bubble(player->current);
        player->current = NULL;
        player->next_bubble = create_bubble(player->launcher_pos, 0);
    }

    // Vérification de la défaite (bulles trop proches du bas)
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (player->grid[i][j] && player->grid[i][j]->pos.y >= WINDOWS_HEIGHT - 100) {
                player->defeat = 1;
                return;
            }
        }
    }
}
