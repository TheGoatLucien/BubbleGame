#include "player.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

// --- Création du joueur et initialisation de la grille ---
player_t create_player(sfVector2f pos) {
    player_t player = { 0 };
    player.launcher_pos = pos;
    player.angle = -3.14f / 2;
    player.score = 0;
    player.defeat = 0;
    player.current = NULL;
    player.falling_bubbles = NULL;
    player.next_bubble = create_bubble(pos, 0);

    float gridOriginX = player.launcher_pos.x - (COLS * H_SPACING) / 2;
    float gridOriginY = 16.0f; // placement initial en Y
    for (int i = 0; i < INIT_ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            bubble_t* b = malloc(sizeof(bubble_t));
            if (!b) continue;
            b->color = rand() % 4 + 1;
            float offsetX = (i % 2 == 0) ? 0 : H_SPACING / 2;
            b->pos.x = gridOriginX + offsetX + j * H_SPACING + BUBBLE_RADIUS;
            b->pos.y = gridOriginY + i * V_SPACING;
            b->active = 0;
            b->falling = 0;
            b->next = NULL;
            player.grid[i][j] = b;
        }
    }
    return player;
}

// --- mise à jour du joueur (orientation et tir) --- 
void update_player(player_t* player, sfEvent event, int left, int right, int shoot) {
    float delta = getDeltaTime();
    float rotationSpeed = 2.5f;
    if (sfKeyboard_isKeyPressed(left))
        player->angle -= rotationSpeed * delta;
    if (sfKeyboard_isKeyPressed(right))
        player->angle += rotationSpeed * delta;
    if (player->angle < -2.5f) player->angle = -2.5f;
    if (player->angle > -0.6f) player->angle = -0.6f;

    if (sfKeyboard_isKeyPressed(shoot) && player->current == NULL) {
        player->current = player->next_bubble;
        player->current->active = 1;
        player->next_bubble = create_bubble(player->launcher_pos, 0);
    }

    if (player->current) {
        player->current->pos.x += cos(player->angle) * 800 * delta;
        player->current->pos.y += sin(player->angle) * 800 * delta;

        // Rebond gauche/droit
        if (player->current->pos.x <= 0 || player->current->pos.x >= WINDOWS_WIDHT) {
            float dx = cos(player->angle), dy = sin(player->angle);
            player->angle = atan2(dy, -dx);
        }

        // On ne traite plus ici le rebond central afin de permettre l'attachement même sans match

        // Si la bulle touche le plafond (défini par CEILING_Y) :
        if (player->current->pos.y <= CEILING_Y)
            goto attach_bubble;

        // Ou si elle entre en collision avec une bulle déjà présente
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                if (player->grid[i][j] &&
                    Circle_Collision(player->current->pos, player->grid[i][j]->pos, BUBBLE_RADIUS, BUBBLE_RADIUS))
                {
                    goto attach_bubble;
                }
            }
        }
    }
    return;

attach_bubble: {
    float gridOriginX = player->launcher_pos.x - (COLS * H_SPACING) / 2;
    int row = (int)fClamp(player->current->pos.y / V_SPACING, 0, ROWS - 1);
    int col;
    if (row % 2 == 0)
        col = (int)fClamp((player->current->pos.x - gridOriginX) / H_SPACING, 0, COLS - 1);
    else
        col = (int)fClamp((player->current->pos.x - (gridOriginX + H_SPACING / 2)) / H_SPACING, 0, COLS - 1);
    float offsetX = (row % 2 == 0) ? 0 : H_SPACING / 2;

    // Recherche d'une cellule vide autour de la cellule cible
    if (!player->grid[row][col]) {
        // Case vide, attache directement
        player->grid[row][col] = player->current;
    }
    else {
        // Si la case est occupée, chercher dans les voisins
        int offsetsEven[6][2] = {
            {0, -1}, {0, 1}, {-1, 0}, {-1, -1}, {1, 0}, {1, -1}
        };
        int offsetsOdd[6][2] = {
            {0, -1}, {0, 1}, {-1, 0}, {-1, 1}, {1, 0}, {1, 1}
        };
        int (*offsets)[2] = (row % 2 == 0) ? offsetsEven : offsetsOdd;
        int attached = 0;
        for (int i = 0; i < 6; i++) {
            int newRow = row + offsets[i][0];
            int newCol = col + offsets[i][1];
            if (newRow >= 0 && newRow < ROWS && newCol >= 0 && newCol < COLS) {
                if (!player->grid[newRow][newCol]) {
                    // Calculer la position en respectant le décalage
                    float newOffset = (newRow % 2 == 0) ? 0 : H_SPACING / 2;
                    player->current->pos.x = gridOriginX + newOffset + newCol * H_SPACING + BUBBLE_RADIUS;
                    player->current->pos.y = newRow * V_SPACING + BUBBLE_RADIUS;
                    player->grid[newRow][newCol] = player->current;
                    attached = 1;
                    break;
                }
            }
        }
        // Si aucun voisin libre n'a été trouvé, forcer l'attachement à la cellule cible
        if (!attached)
            player->grid[row][col] = player->current;
    }
    player->current = NULL;

    // Détection d'un match et explosion si 3 bulles ou plus se connectent
    int visited[ROWS][COLS] = { 0 };
    bubble_t* cluster[ROWS * COLS] = { 0 };
    int count = 0;
    count = flood_fill(player->grid, row, col, player->grid[row][col]->color, visited, cluster, &count);
    if (count >= 3) {
        for (int i = 0; i < count; i++) {
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
    // Une fois l'attachement effectué, on détermine les bulles non rattachées
    drop_floating_bubbles(player);
    }
}

// --- Dessin du joueur, du plafond et du mur central ---
void draw_player(player_t* player, sfRenderWindow* window) {
    // Dessiner le plafond (placé à CEILING_Y)
    sfRectangleShape* ceiling = sfRectangleShape_create();
    sfRectangleShape_setSize(ceiling, (sfVector2f) { WINDOWS_WIDHT, 10 });
    sfRectangleShape_setFillColor(ceiling, sfColor_fromRGBA(80, 80, 80, 255));
    sfRectangleShape_setPosition(ceiling, (sfVector2f) { 0, CEILING_Y - 10 });
    sfRenderWindow_drawRectangleShape(window, ceiling, NULL);
    sfRectangleShape_destroy(ceiling);

    // Dessiner le mur central (visible)
    sfRectangleShape* midWall = sfRectangleShape_create();
    sfRectangleShape_setSize(midWall, (sfVector2f) { 10, WINDOWS_HEIGHT - CEILING_Y });
    sfRectangleShape_setFillColor(midWall, sfColor_fromRGBA(255, 255, 255, 60));
    sfRectangleShape_setPosition(midWall, (sfVector2f) { (WINDOWS_WIDHT / 2) - 5, CEILING_Y });
    sfRenderWindow_drawRectangleShape(window, midWall, NULL);
    sfRectangleShape_destroy(midWall);

    //// Ligne de visée et launcher
    //draw_aim_line(player->launcher_pos, player->angle, window);

    sfCircleShape* launcher = sfCircleShape_create();
    sfCircleShape_setRadius(launcher, 20);
    sfCircleShape_setOrigin(launcher, (sfVector2f) { 20, 20 });
    sfCircleShape_setFillColor(launcher, sfWhite);
    sfCircleShape_setPosition(launcher, player->launcher_pos);
    sfRenderWindow_drawCircleShape(window, launcher, NULL);
    sfCircleShape_destroy(launcher);

    // Dessiner la grille
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (player->grid[i][j])
                draw_bubble(player->grid[i][j], window);
        }
    }

    // Dessiner la bulle en cours de tir ou le preview
    if (player->current)
        draw_bubble(player->current, window);
    else {
        sfCircleShape* preview = sfCircleShape_create();
        sfCircleShape_setRadius(preview, BUBBLE_RADIUS);
        sfCircleShape_setOrigin(preview, (sfVector2f) { BUBBLE_RADIUS, BUBBLE_RADIUS });
        sfCircleShape_setPosition(preview, player->launcher_pos);
        sfColor col;
        switch (player->next_bubble->color) {
        case 1: col = sfRed; break;
        case 2: col = sfBlue; break;
        case 3: col = sfGreen; break;
        case 4: col = sfYellow; break;
        default: col = sfWhite; break;
        }
        sfCircleShape_setFillColor(preview, col);
        sfRenderWindow_drawCircleShape(window, preview, NULL);
        sfCircleShape_destroy(preview);
    }

    // Dessiner les bulles en chute
    bubble_t* iter = player->falling_bubbles;
    while (iter) {
        draw_bubble(iter, window);
        iter = iter->next;
    }
}

// --- Mise à jour des bulles attachées (déclenche l'attache) ---
void update_bubbles(player_t* player) {
    if (!player->current)
        return;

    // Vérifie si la bulle touche le plafond
    if (player->current->pos.y <= CEILING_Y)
        goto attach_bubble;

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (player->grid[i][j] &&
                Circle_Collision(player->current->pos, player->grid[i][j]->pos, BUBBLE_RADIUS, BUBBLE_RADIUS))
            {
                goto attach_bubble;
            }
        }
    }
    return;

attach_bubble: {
    float gridOriginX = player->launcher_pos.x - (COLS * H_SPACING) / 2;
    int row = (int)fClamp(player->current->pos.y / V_SPACING, 0, ROWS - 1);
    int col;
    if (row % 2 == 0)
        col = (int)fClamp((player->current->pos.x - gridOriginX) / H_SPACING, 0, COLS - 1);
    else
        col = (int)fClamp((player->current->pos.x - (gridOriginX + H_SPACING / 2)) / H_SPACING, 0, COLS - 1);
    float offsetX = (row % 2 == 0) ? 0 : H_SPACING / 2;

    if (!player->grid[row][col]) {
        player->grid[row][col] = player->current;
    }
    else {
        int offsetsEven[6][2] = { {0,-1}, {0,1}, {-1,0}, {-1,-1}, {1,0}, {1,-1} };
        int offsetsOdd[6][2] = { {0,-1}, {0,1}, {-1,0}, {-1,1}, {1,0}, {1,1} };
        int (*offsets)[2] = (row % 2 == 0) ? offsetsEven : offsetsOdd;
        int attached = 0;
        for (int i = 0; i < 6; i++) {
            int newRow = row + offsets[i][0];
            int newCol = col + offsets[i][1];
            if (newRow >= 0 && newRow < ROWS && newCol >= 0 && newCol < COLS) {
                if (!player->grid[newRow][newCol]) {
                    float newOffset = (newRow % 2 == 0) ? 0 : H_SPACING / 2;
                    player->current->pos.x = gridOriginX + newOffset + newCol * H_SPACING + BUBBLE_RADIUS;
                    player->current->pos.y = newRow * V_SPACING + BUBBLE_RADIUS;
                    player->grid[newRow][newCol] = player->current;
                    attached = 1;
                    break;
                }
            }
        }
        if (!attached) {  // si aucun voisin libre n'a été trouvé, forcer l'attachement
            player->grid[row][col] = player->current;
        }
    }
    player->current = NULL;

    int visited[ROWS][COLS] = { 0 };
    bubble_t* cluster[ROWS * COLS] = { 0 };
    int count = 0;
    count = flood_fill(player->grid, row, col, player->grid[row][col]->color, visited, cluster, &count);
    if (count >= 3) {
        for (int i = 0; i < count; i++) {
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
    drop_floating_bubbles(player);
    }
}

// --- Recherche de bulles attachées au plafond puis détachement des flottantes ---
static void flood_from_ceiling(bubble_t* grid[ROWS][COLS], int visited[ROWS][COLS]) {
    for (int j = 0; j < COLS; j++) {
        if (grid[0][j] && (grid[0][j]->pos.y - BUBBLE_RADIUS <= CEILING_Y) && !visited[0][j]) {
            int dummyCount = 0;
            bubble_t* dummyCluster[ROWS * COLS] = { 0 };
            flood_fill(grid, 0, j, grid[0][j]->color, visited, dummyCluster, &dummyCount);
        }
    }
}

void drop_floating_bubbles(player_t* player) {
    int visited[ROWS][COLS] = { 0 };
    flood_from_ceiling(player->grid, visited);
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (player->grid[i][j] && !visited[i][j]) {
                player->grid[i][j]->falling = 1;
                player->grid[i][j]->next = player->falling_bubbles;
                player->falling_bubbles = player->grid[i][j];
                player->grid[i][j] = NULL;
            }
        }
    }
}

void update_falling_bubbles(player_t* player) {
    float fallSpeed = 200.0f; // vitesse de chute (pixels/seconde)
    bubble_t** current = &player->falling_bubbles;
    while (*current) {
        (*current)->pos.y += fallSpeed * getDeltaTime();
        if ((*current)->pos.y - BUBBLE_RADIUS > WINDOWS_HEIGHT) {
            bubble_t* toRemove = *current;
            *current = (*current)->next;
            destroy_bubble(toRemove);
        }
        else {
            current = &((*current)->next);
        }
    }
}

// --- Flood fill pour déterminer les groupes ---
int flood_fill(bubble_t* grid[ROWS][COLS], int row, int col, int color, int visited[ROWS][COLS], bubble_t** cluster, int* count) {
    if (row < 0 || row >= ROWS || col < 0 || col >= COLS)
        return 0;
    if (visited[row][col] || !grid[row][col] || grid[row][col]->color != color)
        return 0;
    visited[row][col] = 1;
    cluster[(*count)++] = grid[row][col];

    int offsetsEven[6][2] = { {0,-1}, {0,1}, {-1,0}, {-1,-1}, {1,0}, {1,-1} };
    int offsetsOdd[6][2] = { {0,-1}, {0,1}, {-1,0}, {-1,1}, {1,0}, {1,1} };
    int (*offsets)[2] = (row % 2 == 0) ? offsetsEven : offsetsOdd;
    for (int i = 0; i < 6; i++) {
        int newRow = row + offsets[i][0];
        int newCol = col + offsets[i][1];
        flood_fill(grid, newRow, newCol, color, visited, cluster, count);
    }
    return *count;
}
