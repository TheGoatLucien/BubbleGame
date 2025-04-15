#include "bubble.h"
#include "tools.h"
#include <stdlib.h>
#include <math.h>
#include <time.h>

sfTexture* bubble_texture = NULL;

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
void draw_bubble(bubble_t* bubble, sfRenderWindow* window) {
   

    sfSprite* sprite = sfSprite_create();
    bubble_texture = sfTexture_createFromFile("../Ressources/Textures/bouleA.png", NULL);
    if (!bubble_texture) {
        printf("Erreur chargement texture bulle !\n");
    }
    if (!bubble || !bubble_texture) return;
    sfSprite_setTexture(sprite, bubble_texture, sfTrue);
    sfSprite_setOrigin(sprite, (sfVector2f) { 16, 16 }); // si ta texture fait 32x32 px
    sfSprite_setPosition(sprite, bubble->pos);
    /*sfTexture* shapeTexture;
        shapeTexture = sfTexture_createFromFile("..//Ressources//Textures//boule.png", NULL);*/

    // Définir la couleur en fonction de la couleur de la bulle
    sfColor color;
    switch (bubble->color) {
    case 1: color = sfRed; break;
    case 2: color = sfBlue; break;
    case 3: color = sfGreen; break;
    case 4: color = sfYellow; break;
    default: color = sfWhite; break;
    }

    // Appliquer la transparence uniquement si la bulle est en animation ou en chute
    if (bubble->falling || (bubble->active && bubble->next != NULL)) {
        color.a = 128;
    }
	else {
		color.a = 255;
	}
 
   /* sfCircleShape_setTexture(shape, shapeTexture, sfTrue);*/
    sfSprite_setColor(sprite, color); // Teinte la texture selon la couleur

    sfRenderWindow_drawSprite(window, sprite, NULL);
    sfSprite_destroy(sprite);
}


void destroy_bubble(bubble_t* b) {
    free(b);
}
