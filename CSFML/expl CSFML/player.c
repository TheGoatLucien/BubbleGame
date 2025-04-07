#include "player.h"
#include <math.h>
#include <stdlib.h>

player_t create_player(sfVector2f pos) {
    player_t player = { 0 };
    player.launcher_pos = pos;
    player.angle = -3.14f / 2;
    player.score = 0;
    player.defeat = 0;
    player.current = NULL;

    //  Initialisation de la grille de bulles fixes
    for (int i = 0; i < 5; i++) { // 5 lignes
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
    float rotationSpeed = 2.5f; // radians par seconde

    // Mouvement fluide à gauche/droite avec le temps
    if (sfKeyboard_isKeyPressed(left))
        player->angle -= rotationSpeed * delta;
    if (sfKeyboard_isKeyPressed(right))
        player->angle += rotationSpeed * delta;

    //  Limite les angles de tir
    if (player->angle < -2.5f)
        player->angle = -2.5f;
    if (player->angle > -0.6f)
        player->angle = -0.6f;

    // Tir
    if (sfKeyboard_isKeyPressed(shoot) && player->current == NULL) {
        player->current = create_bubble(player->launcher_pos, player->angle);
    }

    // Déplacement de la bulle
    if (player->current != NULL) {
        player->current->pos.x += cos(player->angle) * 800 * delta;
        player->current->pos.y += sin(player->angle) * 800 * delta;

        // 🎯 Rebond sur bords gauche/droit
        if (player->current->pos.x <= 0 || player->current->pos.x >= WINDOWS_WIDHT) {
            float dx = cos(player->angle);
            float dy = sin(player->angle);
            player->angle = atan2(dy, -dx); // inverser X seulement
        }

        // 🧱 Rebond sur le mur central
        float centerX = WINDOWS_WIDHT / 2;
        float margin = 10;

        if (
            (player->launcher_pos.x < centerX && player->current->pos.x >= centerX - margin) ||
            (player->launcher_pos.x > centerX && player->current->pos.x <= centerX + margin)
            ) {
            float dx = cos(player->angle);
            float dy = sin(player->angle);
            player->angle = atan2(dy, -dx); // inversion horizontale
            if (player->launcher_pos.x < centerX)
                player->current->pos.x = centerX - margin;
            else
                player->current->pos.x = centerX + margin;
        }
		// Si sort par la gauche
        else if (player->launcher_pos.x > 800 && player->current->pos.x <= 800) {
            player->angle = 3.14 - player->angle;
            player->current->pos.x = 801;
        }

        // Si sort par le haut
        if (player->current->pos.y < 0) {
            destroy_bubble(player->current);
            player->current = NULL;
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
    //  Mur central
    sfRectangleShape* midWall = sfRectangleShape_create();
    sfRectangleShape_setSize(midWall, (sfVector2f) { 10, WINDOWS_HEIGHT });
    sfRectangleShape_setFillColor(midWall, sfColor_fromRGBA(255, 255, 255, 60));
    sfRectangleShape_setPosition(midWall, (sfVector2f) { (WINDOWS_WIDHT / 2) - 5, 0 });
    sfRenderWindow_drawRectangleShape(window, midWall, NULL);
    sfRectangleShape_destroy(midWall);

    //  Ligne de visée
    draw_aim_line(player->launcher_pos, player->angle, window);

    //  Le canon
    sfCircleShape* launcher = sfCircleShape_create();
    sfCircleShape_setRadius(launcher, 20);
    sfCircleShape_setOrigin(launcher, (sfVector2f) { 20, 20 });
    sfCircleShape_setFillColor(launcher, sfWhite);
    sfCircleShape_setPosition(launcher, player->launcher_pos);
    sfRenderWindow_drawCircleShape(window, launcher, NULL);
    sfCircleShape_destroy(launcher);

    //  Grille en haut
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (player->grid[i][j]) {
                draw_bubble(player->grid[i][j], window);
            }
        }
    }

    //  Bulle en vol
    if (player->current) {
        draw_bubble(player->current, window);
    }
    else {
        //  Bulle prête à tirer
        sfCircleShape* preview = sfCircleShape_create();
        sfCircleShape_setRadius(preview, 16);
        sfCircleShape_setOrigin(preview, (sfVector2f) { 16, 16 });
        sfCircleShape_setPosition(preview, player->launcher_pos);
        sfCircleShape_setFillColor(preview, sfCyan);
        sfRenderWindow_drawCircleShape(window, preview, NULL);
        sfCircleShape_destroy(preview);
    }
}

void update_bubbles(player_t* player) {
    // TODO : collisions bulles / grille + explosion
}
