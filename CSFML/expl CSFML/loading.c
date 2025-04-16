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
    sfText_setString(text, "Le saviez-vous ?\nLe premier puzzle Bubble a ete publie en 1986.");
    sfFloatRect textBounds = sfText_getLocalBounds(text);
    sfText_setPosition(text, (sfVector2f) { (WINDOWS_WIDHT - textBounds.width) / 2, 150 });

    // Barre de chargement
    sfRectangleShape* loadingBar = sfRectangleShape_create();
    sfRectangleShape_setSize(loadingBar, (sfVector2f) { 0, 30 });
    sfRectangleShape_setFillColor(loadingBar, sfColor_fromRGB(50, 200, 255));
    sfRectangleShape_setPosition(loadingBar, (sfVector2f) { WINDOWS_WIDHT / 4, 300 });

    float duration = 5.0f;  // 5 secondes
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
int ask_ia_level(sfRenderWindow* window) {
    sfFont* font = getDefaultFont();
    sfText* prompt = sfText_create();
    sfText_setFont(prompt, font);
    sfText_setString(prompt, "Choisissez le niveau de l IA (1 A 5) :");
    sfText_setCharacterSize(prompt, 28);
    sfText_setPosition(prompt, (sfVector2f) { 100, 100 });

    sfText* choices[5];
    for (int i = 0; i < 5; i++) {
        choices[i] = sfText_create();
        char label[2] = { '1' + i, '\0' };
        sfText_setFont(choices[i], font);
        sfText_setString(choices[i], label);
        sfText_setCharacterSize(choices[i], 40);
        sfText_setPosition(choices[i], (sfVector2f) { 150 + i * 60, 200 });
    }

    while (sfRenderWindow_isOpen(window)) {
        sfEvent event;
        while (sfRenderWindow_pollEvent(window, &event)) {
            if (event.type == sfEvtClosed) sfRenderWindow_close(window);
            if (event.type == sfEvtMouseButtonPressed) {
                sfVector2f mouse = { event.mouseButton.x, event.mouseButton.y };
                for (int i = 0; i < 5; i++) {
                    sfFloatRect bounds = sfText_getGlobalBounds(choices[i]);
                    if (sfFloatRect_contains(&bounds, mouse.x, mouse.y)) {
                        return i + 1;
                    }

                }
            }
        }

        sfRenderWindow_clear(window, sfBlack);
        sfRenderWindow_drawText(window, prompt, NULL);
        for (int i = 0; i < 5; i++) sfRenderWindow_drawText(window, choices[i], NULL);
        sfRenderWindow_display(window);
    }

    return 1; // Par défaut
}
