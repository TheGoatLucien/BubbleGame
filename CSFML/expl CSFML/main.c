#include "tools.h"



int main() {
    // Initialisation
    initTools();
    sfRenderWindow* window = sfRenderWindow_create((sfVideoMode) { 800, 600, 32 }, "Simple", sfResize | sfClose, NULL);
    sfEvent event;
 


    while (sfRenderWindow_isOpen(window)) {
        restartClock();

        // Gestion des événements
        while (sfRenderWindow_pollEvent(window, &event)) {
            if (event.type == sfEvtClosed) {
                sfRenderWindow_close(window);
            }
        }

        // update
        

        // Affichage
        sfRenderWindow_clear(window, sfBlack);
  
        sfRenderWindow_display(window);
    }

    // Libération des ressources
  
    sfRenderWindow_destroy(window);

    return 0;
}