#pragma once
#include "tools.h"

#include <SFML/Graphics.h>

typedef struct bubble {
    sfVector2f pos;
    int color;
    int active;
    int falling;       // 0 = non, 1 = tombe
    struct bubble* next;
} bubble_t;

bubble_t* create_bubble(sfVector2f start, float angle);  //init des bulles
void draw_bubble(bubble_t* b, sfRenderWindow* window); //dessin
void destroy_bubble(bubble_t* b);