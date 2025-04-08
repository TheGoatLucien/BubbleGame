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
    player.next_bubble = create_bubble(pos, 0); // Initialisation de la prochaine bulle

    // Init grille
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < COLS; j++) {
            bubble_t* b = malloc(sizeof(bubble_t));
            b->color = rand() % 4 + 1;
            b->pos = (sfVector2f){
                player.launcher_pos.x - (COLS * 32) / 2 + j * 32,
                50 + i * 28
            };
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

int flood_fill(bubble_t* grid[ROWS][COLS], int row, int col, int color, int visited[ROWS][COLS], bubble_t* cluster[ROWS * COLS]) {
    if (row < 0 || row >= ROWS || col < 0 || col >= COLS)
        return 0;
    if (visited[row][col] || !grid[row][col] || grid[row][col]->color != color)
        return 0;

    visited[row][col] = 1;
    cluster[0] = grid[row][col];
    int count = 1;

    int dx[6] = { -1, 1, 0, 0, -1, 1 };
    int dy[6] = { 0, 0, -1, 1, 1, -1 };

    for (int d = 0; d < 6; d++) {
        bubble_t* subcluster[ROWS * COLS] = { 0 };
        int subcount = flood_fill(grid, row + dx[d], col + dy[d], color, visited, subcluster);
        for (int i = 0; i < subcount; i++) {
            cluster[count++] = subcluster[i];
        }
    }

    return count;
}

void update_bubbles(player_t* player) {
    if (!player->current) return;

    // Collision plafond
    if (player->current->pos.y <= 0) goto attach_bubble;

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (player->grid[i][j] &&
                Circle_Collision(player->current->pos, player->grid[i][j]->pos, 16, 16)) {
                goto attach_bubble;
            }
        }
    }

    return;

attach_bubble:;
    float grid_origin_x = player->launcher_pos.x - (COLS * 32) / 2;
    int col = fClamp((player->current->pos.x - grid_origin_x) / 32, 0, COLS - 1);
    int row = fClamp(player->current->pos.y / 28, 0, ROWS - 1);

    if (!player->grid[row][col]) {
        player->grid[row][col] = player->current;
        player->grid[row][col]->pos.x = grid_origin_x + col * 32 + 16;
        player->grid[row][col]->pos.y = row * 28 + 16;
        player->current = NULL;

        // Check et explosion des grappes
        int visited[ROWS][COLS] = { 0 };
        bubble_t* cluster[ROWS * COLS] = { 0 };
        int count = flood_fill(player->grid, row, col, player->grid[row][col]->color, visited, cluster);

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
    }
    else {
        destroy_bubble(player->current);
        player->current = NULL;
        player->next_bubble = create_bubble(player->launcher_pos, 0); // Nouvelle bulle après destruction
    }

    // Condition de défaite : bulles trop proches du joueur
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (player->grid[i][j] && player->grid[i][j]->pos.y >= WINDOWS_HEIGHT - 100) {
                player->defeat = 1;
                return;
            }
        }
    }
}
