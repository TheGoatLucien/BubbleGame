#include "player.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define BUBBLE_RADIUS 16
#define GRID_CELL_WIDTH 32
#define GRID_CELL_HEIGHT 28

player_t create_player(sfVector2f pos) {
    player_t player = { 0 };
    player.launcher_pos = pos;
    player.angle = -3.14f / 2;
    player.score = 0;
    player.defeat = 0;
    player.current = NULL;
    player.next_bubble = create_bubble(pos, 0);
    player.combo_count = 0;

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < COLS; j++) {
            bubble_t* b = malloc(sizeof(bubble_t));
            b->color = rand() % 4 + 1;
            b->pos = (sfVector2f){
                player.launcher_pos.x - (COLS * 32) / 2 + j * 32 + (i % 2 == 1 ? 16 : 0),
                50 + i * 28
            };
            b->active = 0;
            b->next = NULL;
            player.grid[i][j] = b;
        }
    }

    check_initial_matches(&player);
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
        player->next_bubble = create_bubble(player->launcher_pos, 0);
    }

    if (player->current != NULL) {
        player->current->pos.x += cos(player->angle) * 800 * delta;
        player->current->pos.y += sin(player->angle) * 800 * delta;

        if (player->current->pos.x <= 0 || player->current->pos.x >= WINDOWS_WIDHT) {
            float dx = cos(player->angle);
            float dy = sin(player->angle);
            player->angle = atan2(dy, -dx);
        }

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

        if (player->current->pos.y < 0) {
            destroy_bubble(player->current);
            player->current = NULL;
            player->next_bubble = create_bubble(player->launcher_pos, 0);
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
    if (visited[row][col] || !grid[row][col])
        return 0;
    if (color != -1 && grid[row][col]->color != color) // -1 utilisé pour detect_floating_bubbles
        return 0;

    visited[row][col] = 1;
    int cluster_index = 0;
    for (int i = 0; i < ROWS * COLS; i++) {
        if (cluster[i] == NULL) {
            cluster_index = i;
            break;
        }
    }
    cluster[cluster_index] = grid[row][col];
    int count = 1;

    int dx[6] = { -1, 1, 0, 0, -1, 1 };
    int dy[6] = { 0, 0, -1, 1, 1, -1 };
    if (row % 2 == 0) { // Ajustement pour grille hexagonale
        dx[4] = -1; dx[5] = 1;
    }
    else {
        dx[4] = 0; dx[5] = 0;
    }

    for (int d = 0; d < 6; d++) {
        int new_row = row + dx[d];
        int new_col = col + dy[d];
        if (new_row < 0 || new_row >= ROWS || new_col < 0 || new_col >= COLS)
            continue;
        if (!visited[new_row][new_col] && grid[new_row][new_col]) {
            int subcount = flood_fill(grid, new_row, new_col, color, visited, cluster);
            count += subcount;
        }
    }

    return count;
}

void check_initial_matches(player_t* player) {
    int visited[ROWS][COLS] = { 0 };
    bubble_t* cluster[ROWS * COLS] = { 0 };

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (player->grid[i][j] && !visited[i][j]) {
                int count = flood_fill(player->grid, i, j, player->grid[i][j]->color, visited, cluster);
                if (count >= 3) {
                    for (int k = 0; k < ROWS * COLS; k++) {
                        if (cluster[k]) {
                            for (int r = 0; r < ROWS; r++) {
                                for (int c = 0; c < COLS; c++) {
                                    if (player->grid[r][c] == cluster[k]) {
                                        destroy_bubble(player->grid[r][c]);
                                        player->grid[r][c] = NULL;
                                    }
                                }
                            }
                            cluster[k] = NULL;
                        }
                    }
                }
                memset(cluster, 0, sizeof(bubble_t*) * ROWS * COLS);
            }
        }
    }
}

void snap_to_grid(player_t* player, bubble_t* bubble, float grid_origin_x) {
    int row = round(bubble->pos.y / GRID_CELL_HEIGHT);
    if (row < 0) row = 0;
    if (row >= ROWS) row = ROWS - 1;

    float offset_x = (row % 2 == 1) ? GRID_CELL_WIDTH / 2 : 0;
    int col = round((bubble->pos.x - grid_origin_x - offset_x) / GRID_CELL_WIDTH);
    if (col < 0) col = 0;
    if (col >= COLS) col = COLS - 1;

    float best_dist = 999999.0f;
    int best_row = row;
    int best_col = col;

    for (int r = row - 1; r <= row + 1; r++) {
        if (r < 0 || r >= ROWS) continue;
        for (int c = col - 1; c <= col + 1; c++) {
            if (c < 0 || c >= COLS) continue;
            if (player->grid[r][c]) continue;

            float offset = (r % 2 == 1) ? GRID_CELL_WIDTH / 2 : 0;
            float grid_x = grid_origin_x + c * GRID_CELL_WIDTH + offset + BUBBLE_RADIUS;
            float grid_y = r * GRID_CELL_HEIGHT + BUBBLE_RADIUS;
            float dist = sqrt(pow(bubble->pos.x - grid_x, 2) + pow(bubble->pos.y - grid_y, 2));

            if (dist < best_dist) {
                best_dist = dist;
                best_row = r;
                best_col = c;
            }
        }
    }

    float final_offset = (best_row % 2 == 1) ? GRID_CELL_WIDTH / 2 : 0;
    bubble->pos.x = grid_origin_x + best_col * GRID_CELL_WIDTH + final_offset + BUBBLE_RADIUS;
    bubble->pos.y = best_row * GRID_CELL_HEIGHT + BUBBLE_RADIUS;

    player->grid[best_row][best_col] = bubble;
}

void detect_floating_bubbles(player_t* player) {
    int visited[ROWS][COLS] = { 0 };
    bubble_t* cluster[ROWS * COLS] = { 0 };

    for (int col = 0; col < COLS; col++) {
        if (player->grid[0][col] && !visited[0][col]) {
            flood_fill(player->grid, 0, col, -1, visited, cluster);
            memset(cluster, 0, sizeof(bubble_t*) * ROWS * COLS);
        }
    }

    int floating_count = 0;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (player->grid[i][j] && !visited[i][j]) {
                destroy_bubble(player->grid[i][j]);
                player->grid[i][j] = NULL;
                floating_count++;
            }
        }
    }

    if (floating_count > 0) {
        player->combo_count++;
        player->score += floating_count * 10 * player->combo_count;
    }
}

void update_bubbles(player_t* player) {
    if (!player->current) return;

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
    snap_to_grid(player, player->current, grid_origin_x);
    player->current = NULL;

    player->combo_count = 0;

    int visited[ROWS][COLS] = { 0 };
    bubble_t* cluster[ROWS * COLS] = { 0 };
    int did_match = 0;

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (player->grid[i][j] && !visited[i][j]) {
                int count = flood_fill(player->grid, i, j, player->grid[i][j]->color, visited, cluster);
                if (count >= 3) {
                    did_match = 1;
                    player->combo_count++;
                    for (int k = 0; k < ROWS * COLS; k++) {
                        if (cluster[k]) {
                            for (int r = 0; r < ROWS; r++) {
                                for (int c = 0; c < COLS; c++) {
                                    if (player->grid[r][c] == cluster[k]) {
                                        destroy_bubble(player->grid[r][c]);
                                        player->grid[r][c] = NULL;
                                    }
                                }
                            }
                            cluster[k] = NULL;
                        }
                    }
                    player->score += count * 10 * player->combo_count;
                }
                memset(cluster, 0, sizeof(bubble_t*) * ROWS * COLS);
            }
        }
    }

    if (did_match) {
        detect_floating_bubbles(player);
    }

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (player->grid[i][j] && player->grid[i][j]->pos.y >= WINDOWS_HEIGHT - 100) {
                player->defeat = 1;
                return;
            }
        }
    }
}