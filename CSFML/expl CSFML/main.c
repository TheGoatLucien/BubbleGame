#include "tools.h"
#include "bubble.h"
#include "player.h"

void send_grey_bubbles(player_t* sender, player_t* receiver) {
    int amount = sender->score / 3;
    if (amount <= 0) return;

    for (int i = 0; i < amount; i++) {
        int col = rand() % COLS;

        // Trouver la première position libre en partant du haut
        int first_empty_row = -1;
        for (int row = 0; row < ROWS; row++) {
            if (!receiver->grid[row][col]) {
                first_empty_row = row;
                break;
            }
        }

        // Si la colonne est pleine, on déclare la défaite
        if (first_empty_row == -1) {
            receiver->defeat = 1;
            return;
        }

        // Pousser les bulles vers le bas
        for (int row = ROWS - 1; row > first_empty_row; row--) {
            receiver->grid[row][col] = receiver->grid[row - 1][col];
            if (receiver->grid[row][col])
                receiver->grid[row][col]->pos.y = row * V_SPACING + BUBBLE_RADIUS;
        }

        // Crée la bulle envoyée avec une couleur valide (1 à 4)
        bubble_t* adversaryBubble = malloc(sizeof(bubble_t));
        adversaryBubble->color = rand() % 4 + 1;  // couleur jouable
        adversaryBubble->active = 0;
        adversaryBubble->next = NULL;

        float gridOriginX = receiver->launcher_pos.x - (COLS * H_SPACING) / 2;
        float offsetX = (first_empty_row % 2 == 0) ? 0 : H_SPACING / 2;
        adversaryBubble->pos.x = gridOriginX + offsetX + col * H_SPACING + BUBBLE_RADIUS;
        adversaryBubble->pos.y = first_empty_row * V_SPACING + BUBBLE_RADIUS;

        receiver->grid[first_empty_row][col] = adversaryBubble;
    }

    sender->score = 0;
}


int main() {
    initTools();
    initButton();

    sfRenderWindow* window = sfRenderWindow_create(
        (sfVideoMode) {
        WINDOWS_WIDHT, WINDOWS_HEIGHT, 32
    },
        "BubleX", sfResize | sfClose, NULL);
    sfEvent event;
    gameState = MENU;

    player_t p1 = create_player((sfVector2f) { WINDOWS_WIDHT / 4, WINDOWS_HEIGHT - 50 });
    player_t p2 = create_player((sfVector2f) { (3 * WINDOWS_WIDHT) / 4, WINDOWS_HEIGHT - 50 });
    sfVector2i mousePosition;
    float time_elapsed = 0;
    float generation_interval = 5.0f; // Une nouvelle ligne toutes les 5 secondes
    while (sfRenderWindow_isOpen(window)) {
        restartClock();

        while (sfRenderWindow_pollEvent(window, &event)) {
            if (event.type == sfEvtClosed) {
                sfRenderWindow_close(window);
            }

            if (event.type == sfEvtMouseButtonPressed) {
                mousePosition = sfMouse_getPositionRenderWindow(window);
                if (gameState == MENU) {
                    updateButton(window, mousePosition);
                }
            }

            if (event.type == sfEvtMouseMoved) {
                mousePosition = sfMouse_getPositionRenderWindow(window);
                updateLightBtn(window, mousePosition);
            }
        }

        if (gameState != MENU && sfKeyboard_isKeyPressed(sfKeyW)) {
            gameState = MENU;
        }

        if (gameState == GAME) {
            time_elapsed += getDeltaTime();

            if (time_elapsed >= generation_interval) {
                add_random_bubble_line(&p1); // Ajoute une ligne pour le joueur 1
                add_random_bubble_line(&p2); // Ajoute une ligne pour le joueur 2
                time_elapsed = 0; // Réinitialiser le timer
            }
            update_player(&p1, event, sfKeyQ, sfKeyD, sfKeySpace);
            update_player(&p2, event, sfKeyLeft, sfKeyRight, sfKeyReturn);

            update_bubbles(&p1);
            update_bubbles(&p2);

            send_grey_bubbles(&p1, &p2);
            send_grey_bubbles(&p2, &p1);
        }

        sfRenderWindow_clear(window, sfBlack);

        if (gameState == MENU) {
            displayButton(window);
        }
        else if (gameState == GAME) {
            draw_player(&p1, window);
            draw_player(&p2, window);

            if (p1.defeat || p2.defeat) {
                sfFont* font = getDefaultFont();
                sfText* text = sfText_create();
                sfText_setFont(text, font);
                sfText_setCharacterSize(text, 60);
                sfText_setFillColor(text, sfRed);

                if (p1.defeat)
                    sfText_setString(text, "JOUEUR 2 A GAGNE !");
                else
                    sfText_setString(text, "JOUEUR 1 A GAGNE !");

                sfFloatRect bounds = sfText_getLocalBounds(text);
                sfVector2f center = { (WINDOWS_WIDHT - bounds.width) / 2, (WINDOWS_HEIGHT - bounds.height) / 2 };
                sfText_setPosition(text, center);

                sfRenderWindow_drawText(window, text, NULL);
                sfRenderWindow_display(window);
                sfSleep(sfSeconds(4));

                sfText_destroy(text);

                gameState = MENU;
                p1 = create_player((sfVector2f) { WINDOWS_WIDHT / 4, WINDOWS_HEIGHT - 50 });
                p2 = create_player((sfVector2f) { (3 * WINDOWS_WIDHT) / 4, WINDOWS_HEIGHT - 50 });
                continue;
            }
        }
        else if (gameState == OPTION) {
            // Affichage des options
        }

        sfRenderWindow_display(window);
    }

    if (gameState == MENU) {
        destroyButton();
    }

    sfRenderWindow_destroy(window);
    return 0;
}