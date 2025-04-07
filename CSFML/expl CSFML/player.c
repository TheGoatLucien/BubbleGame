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

    // 🔒 Limite les angles de tir
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

        if (player->current->pos.y < 0 ||
            player->current->pos.x < 0 ||
            player->current->pos.x > WINDOWS_WIDHT) {
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
    // TODO : logique de collision avec la grille
}
