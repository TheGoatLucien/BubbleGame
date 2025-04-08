#pragma once
#include "tools.h"
#include <SFML/Graphics.h>

typedef struct bubble {
    sfVector2f pos;
    int color;
    int active;
    int falling; // 0 : attach�e, 1 : en chute
    struct bubble* next;
} bubble_t;

bubble_t* create_bubble(sfVector2f start, float angle);
void draw_bubble(bubble_t* b, sfRenderWindow* window);
void destroy_bubble(bubble_t* b);
