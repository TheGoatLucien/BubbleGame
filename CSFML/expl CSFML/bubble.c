#include "bubble.h"
#include <stdlib.h>
#include <math.h>
#include <time.h>

bubble_t* create_bubble(sfVector2f start, float angle) {
    bubble_t* b = malloc(sizeof(bubble_t));
    if (!b) return NULL;
    b->pos = start;
    b->color = rand() % 4 + 1; // couleur jouable entre 1 et 4
    b->active = 1;
    b->falling = 0;
    b->next = NULL;
    return b;
}

void draw_bubble(bubble_t* b, sfRenderWindow* window) {
    sfCircleShape* shape = sfCircleShape_create();
    sfCircleShape_setRadius(shape, 16);
    sfCircleShape_setOrigin(shape, (sfVector2f) { 16, 16 });
    sfCircleShape_setPosition(shape, b->pos);

    sfColor col;
    switch (b->color) {
    case 1: col = sfRed; break;
    case 2: col = sfBlue; break;
    case 3: col = sfGreen; break;
    case 4: col = sfYellow; break;
    default: col = sfWhite; break;
    }
    sfCircleShape_setFillColor(shape, col);
    sfRenderWindow_drawCircleShape(window, shape, NULL);
    sfCircleShape_destroy(shape);
}

void destroy_bubble(bubble_t* b) {
    free(b);
}
