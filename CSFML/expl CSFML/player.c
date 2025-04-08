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
    player.next_bubble = create_bubble(pos, 0);
    player.falling_bubbles = NULL;

    float gridOriginX = pos.x - (COLS * H_SPACING) / 2;
    for (int i = 0; i < INIT_ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            bubble_t* b = malloc(sizeof(bubble_t));
            if (!b) continue;
            b->color = rand() % 4 + 1;
            float offset = (i % 2 == 0) ? 0 : H_SPACING / 2;
            b->pos.x = gridOriginX + offset + j * H_SPACING + BUBBLE_RADIUS;
            b->pos.y = CEILING_Y + 10 + i * V_SPACING;
            b->active = 0;
            b->falling = 0;
            b->next = NULL;
            player.grid[i][j] = b;
        }
    }

    return player;
}

void update_player(player_t* player, sfEvent event, int left, int right, int shoot) {
    float delta = getDeltaTime();
    float speed = 2.5f;

    if (sfKeyboard_isKeyPressed(left)) player->angle -= speed * delta;
    if (sfKeyboard_isKeyPressed(right)) player->angle += speed * delta;
    if (player->angle < -2.5f) player->angle = -2.5f;
    if (player->angle > -0.6f) player->angle = -0.6f;

    if (sfKeyboard_isKeyPressed(shoot) && player->current == NULL) {
        player->current = player->next_bubble;
        player->current->active = 1;
        player->next_bubble = create_bubble(player->launcher_pos, 0);
    }

    if (!player->current) return;

    player->current->pos.x += cos(player->angle) * 800 * delta;
    player->current->pos.y += sin(player->angle) * 800 * delta;

    if (player->current->pos.x <= 0 || player->current->pos.x >= WINDOWS_WIDHT) {
        float dx = cos(player->angle), dy = sin(player->angle);
        player->angle = atan2(dy, -dx);
    }
}

int flood_fill(bubble_t* grid[ROWS][COLS], int row, int col, int color,
    int visited[ROWS][COLS], bubble_t** cluster, int* count) {
    if (row < 0 || row >= ROWS || col < 0 || col >= COLS)
        return 0;
    if (visited[row][col] || !grid[row][col] || grid[row][col]->color != color)
        return 0;

    visited[row][col] = 1;
    cluster[(*count)++] = grid[row][col];

    int offsetsEven[6][2] = { {0,-1}, {0,1}, {-1,0}, {-1,-1}, {1,0}, {1,-1} };
    int offsetsOdd[6][2] = { {0,-1}, {0,1}, {-1,0}, {-1,1},  {1,0}, {1,1} };
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

    // Collision avec le plafond
    if (player->current->pos.y <= CEILING_Y)
        goto attach;

    // Collision avec une autre bulle
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (player->grid[i][j] &&
                Circle_Collision(player->current->pos, player->grid[i][j]->pos, BUBBLE_RADIUS, BUBBLE_RADIUS)) {
                goto attach;
            }
        }
    }

    return;

attach: {
    float gridOriginX = player->launcher_pos.x - (COLS * H_SPACING) / 2;
    int row = (int)fClamp((player->current->pos.y - CEILING_Y) / V_SPACING, 0, ROWS - 1);
    int col = (int)fClamp((player->current->pos.x - gridOriginX) / H_SPACING, 0, COLS - 1);

    float offsetX = (row % 2 == 0) ? 0 : H_SPACING / 2;
    player->current->pos.x = gridOriginX + offsetX + col * H_SPACING + BUBBLE_RADIUS;
    player->current->pos.y = CEILING_Y + row * V_SPACING;

    if (!player->grid[row][col]) {
        player->grid[row][col] = player->current;
    }
    else {
        destroy_bubble(player->current);  // fallback
    }

    player->current = NULL;

    // Explosion
    int visited[ROWS][COLS] = { 0 };
    bubble_t* cluster[ROWS * COLS] = { 0 };
    int count = 0;
    flood_fill(player->grid, row, col, player->grid[row][col]->color, visited, cluster, &count);

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
        drop_floating_bubbles(player);
    }

    }
}


void drop_floating_bubbles(player_t* player) {
    int visited[ROWS][COLS] = { 0 };

    // Marquer les bulles reliées au plafond
    for (int j = 0; j < COLS; j++) {
        if (player->grid[0][j] && !visited[0][j]) {
            int dummyCount = 0;
            bubble_t* dummyCluster[ROWS * COLS] = { 0 };
            flood_fill(player->grid, 0, j, player->grid[0][j]->color, visited, dummyCluster, &dummyCount);
        }
    }

    // Toutes les bulles non visitées tombent
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

void update_falling_bubbles(player_t * player) {
    float fallSpeed = 200.0f;
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

void draw_aim_line(sfVector2f origin, float angle, sfRenderWindow * window) {
    sfVector2f pos = origin;
    float dx = cos(angle), dy = sin(angle);

    sfVertexArray* line = sfVertexArray_create();
    sfVertexArray_setPrimitiveType(line, sfLinesStrip);
    sfVertex start = { pos, sfColor_fromRGBA(255, 255, 255, 60) };
    sfVertexArray_append(line, start);

    for (int bounce = 0; bounce < 2; bounce++) {
        for (int i = 0; i < 100; i++) {
            pos.x += dx * 5;
            pos.y += dy * 5;

            if (pos.x <= 0 || pos.x >= WINDOWS_WIDHT) {
                dx = -dx;
                break;
            }
            if (pos.y <= CEILING_Y) {
                dy = -dy;
                break;
            }
        }

        sfVertex v = { pos, sfColor_fromRGBA(255, 255, 255, 60) };
        sfVertexArray_append(line, v);
    }

    sfRenderWindow_drawVertexArray(window, line, NULL);
    sfVertexArray_destroy(line);
}

void draw_player(player_t * player, sfRenderWindow * window) {
    // plafond
    sfRectangleShape* ceiling = sfRectangleShape_create();
    sfRectangleShape_setSize(ceiling, (sfVector2f) { WINDOWS_WIDHT, 10 });
    sfRectangleShape_setFillColor(ceiling, sfColor_fromRGBA(80, 80, 80, 255));
    sfRectangleShape_setPosition(ceiling, (sfVector2f) { 0, CEILING_Y - 10 });
    sfRenderWindow_drawRectangleShape(window, ceiling, NULL);
    sfRectangleShape_destroy(ceiling);

    // ligne de visée
    draw_aim_line(player->launcher_pos, player->angle, window);

    // canon
    sfCircleShape* launcher = sfCircleShape_create();
    sfCircleShape_setRadius(launcher, 20);
    sfCircleShape_setOrigin(launcher, (sfVector2f) { 20, 20 });
    sfCircleShape_setFillColor(launcher, sfWhite);
    sfCircleShape_setPosition(launcher, player->launcher_pos);
    sfRenderWindow_drawCircleShape(window, launcher, NULL);
    sfCircleShape_destroy(launcher);

    // grille
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (player->grid[i][j])
                draw_bubble(player->grid[i][j], window);
        }
    }

    // bulle en vol
    if (player->current) {
        draw_bubble(player->current, window);
    }
    else if (player->next_bubble) {
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

    // bulles en chute
    bubble_t* fall = player->falling_bubbles;
    while (fall) {
        draw_bubble(fall, window);
        fall = fall->next;
    }
}
