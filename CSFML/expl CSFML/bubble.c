#include "bubble.h"
#include <stdlib.h>
#include <math.h>
#include <time.h>

bubble_t* create_bubble(sfVector2f start, float angle) {
    bubble_t* b = malloc(sizeof(bubble_t));
    if (!b) return NULL;
    b->pos = start;
    b->color = rand() % 4 + 1; // 1 à 4
    b->active = 1;
    b->next = NULL;
	b->falling = 0; // Initialement pas en chute
    return b;
}
void draw_bubble(bubble_t* b, sfRenderWindow* window) {
    sfCircleShape* shape = sfCircleShape_create();
    sfCircleShape_setRadius(shape, 16);
    sfCircleShape_setOrigin(shape, (sfVector2f) { 16, 16 });
    sfCircleShape_setPosition(shape, b->pos);

    sfColor color;
    switch (b->color) {
    case 1: color = sfRed; break;
    case 2: color = sfBlue; break;
    case 3: color = sfGreen; break;
    case 4: color = sfYellow; break;
    default: color = sfWhite; break;
    }
    if (b->falling || b->active) {
        color.a = 128; // Semi-transparente si en chute ou en animation
    }

    sfCircleShape_setFillColor(shape, color);
    sfRenderWindow_drawCircleShape(window, shape, NULL);
    sfCircleShape_destroy(shape);
}

void destroy_bubble(bubble_t* b) {
    free(b);
}
