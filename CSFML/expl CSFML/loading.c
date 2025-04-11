#include "loading.h"
#include <stdio.h>
#include <stdlib.h>

void show_loading_screen(sfRenderWindow* window) {
    sfClock* clock = sfClock_create();
    sfFont* font = getDefaultFont();  
    if (!font) return;

    // Texte "Le saviez-vous ?"
    sfText* text = sfText_create();
    sfText_setFont(text, font);
    sfText_setCharacterSize(text, 28);
    sfText_setColor(text, sfWhite);
    sfText_setString(text, "Le saviez-vous ?\nLe premier jeu puzzle game est faite en 1980.");
    sfFloatRect textBounds = sfText_getLocalBounds(text);
    sfText_setPosition(text, (sfVector2f) { (WINDOWS_WIDHT - textBounds.width) / 2, 150 });

    // Barre de chargement
    sfRectangleShape* loadingBar = sfRectangleShape_create();
    sfRectangleShape_setSize(loadingBar, (sfVector2f) { 0, 30 });
    sfRectangleShape_setFillColor(loadingBar, sfColor_fromRGB(50, 200, 255));
    sfRectangleShape_setPosition(loadingBar, (sfVector2f) { WINDOWS_WIDHT / 4, 300 });

    float duration = 1.0f;  // 5 secondes
    float maxWidth = WINDOWS_WIDHT / 2;

    while (sfClock_getElapsedTime(clock).microseconds / 1000000.0f < duration) {
        sfRenderWindow_clear(window, sfBlack);

        float elapsed = sfClock_getElapsedTime(clock).microseconds / 1000000.0f;
        float progress = elapsed / duration;
        sfRectangleShape_setSize(loadingBar, (sfVector2f) { maxWidth* progress, 30 });

        // Affichage
        sfRenderWindow_drawText(window, text, NULL);
        sfRenderWindow_drawRectangleShape(window, loadingBar, NULL);
        sfRenderWindow_display(window);
    }

    sfClock_destroy(clock);
    sfText_destroy(text);
    sfRectangleShape_destroy(loadingBar);
    
}
