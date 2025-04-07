#include "tools.h"

#include "bubble.h"
#include "player.h"

int main() {
    // Initialisation
    initTools();
    initButton(); //initialise le menu

    sfRenderWindow* window = sfRenderWindow_create((sfVideoMode) { WINDOWS_WIDHT, WINDOWS_HEIGHT, 32 }, "BubleX", sfResize | sfClose, NULL);
    sfEvent event;
    gameState = 0;//init menu

    player_t p1 = create_player((sfVector2f) { WINDOWS_WIDHT / 4, WINDOWS_HEIGHT - 50 });
    player_t p2 = create_player((sfVector2f) { (3 * WINDOWS_WIDHT) / 4, WINDOWS_HEIGHT - 50 });
    sfVector2i mousePosition;
 


    while (sfRenderWindow_isOpen(window)) {
        restartClock();

        // Gestion des événements
        while (sfRenderWindow_pollEvent(window, &event)) {
            if (event.type == sfEvtClosed) {
                sfRenderWindow_close(window);
            }
            //positionn de la souris
            if (event.type == sfEvtMouseButtonPressed) {
                
                //musicClick(audio); //joue un song à chaque click
                mousePosition = sfMouse_getPositionRenderWindow(window);
                if (gameState == 0) {
                    updateButton(window, mousePosition);//pour verfier quel bouton a choisi le joueur et puis renvoyer la reponse respective à son choix
                }

                
            }
            if (event.type == sfEvtMouseMoved) {
                mousePosition = sfMouse_getPositionRenderWindow(window);
                updateLightBtn(window, mousePosition);
            }
        }

        if (gameState != 0) {//debug
            if (sfKeyboard_isKeyPressed(sfKeyW)) { //retourner au menu
                gameState = 0;

            }

        }
        
        if (gameState == GAME) {
            update_player(&p1, event, sfKeyQ, sfKeyD, sfKeySpace);
            update_player(&p2, event, sfKeyLeft, sfKeyRight, sfKeyReturn);

            update_bubbles(&p1);
            update_bubbles(&p2);
        }
		else if (gameState == OPTION) {
			//option
		}
		else {
			//menu

        }





        // Affichage
        sfRenderWindow_clear(window, sfBlack);
  
        if (gameState == MENU) {
            displayButton(window);
        }
        else if (gameState == GAME) {
                draw_player(&p1, window);
                draw_player(&p2, window);

        }
        else { //option

        }



        sfRenderWindow_display(window);
    }

    // Libération des ressources
    if (gameState == MENU) {
        destroyButton();
    }
    else if (gameState == GAME) {

    }
    else { //option

    }
  
    sfRenderWindow_destroy(window);

    return 0;
}