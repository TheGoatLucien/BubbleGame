#include "tools.h"
#include "bubble.h"
#include "player.h"

void send_grey_bubbles(player_t* sender, player_t* receiver) {
    int amount = sender->score / 3;
    if (amount <= 0) return;

    for (int i = 0; i < amount; i++) {
        int col = rand() % COLS;
        int first_empty_row = -1;
        for (int row = 0; row < ROWS; row++) {
            if (!receiver->grid[row][col]) {
                first_empty_row = row;
                break;
            }
        }
        if (first_empty_row == -1) {
            receiver->defeat = 1;
            return;
        }
        for (int row = ROWS - 1; row > first_empty_row; row--) {
            receiver->grid[row][col] = receiver->grid[row - 1][col];
            if (receiver->grid[row][col])
                receiver->grid[row][col]->pos.y = row * V_SPACING + BUBBLE_RADIUS;
        }
        bubble_t* bubble_adv = malloc(sizeof(bubble_t));
        bubble_adv->color = rand() % 4 + 1; // couleur jouable
        bubble_adv->active = 0;
        bubble_adv->falling = 0;
        bubble_adv->next = NULL;
        float grid_origin_x = receiver->launcher_pos.x - (COLS * H_SPACING) / 2;
        float offset = (first_empty_row % 2 == 0) ? 0 : H_SPACING / 2;
        bubble_adv->pos.x = grid_origin_x + offset + col * H_SPACING + BUBBLE_RADIUS;
        bubble_adv->pos.y = first_empty_row * V_SPACING + BUBBLE_RADIUS;
        receiver->grid[first_empty_row][col] = bubble_adv;
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

    while (sfRenderWindow_isOpen(window)) {
        restartClock();
        while (sfRenderWindow_pollEvent(window, &event)) {
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);
            if (event.type == sfEvtMouseButtonPressed) {
                mousePosition = sfMouse_getPositionRenderWindow(window);
                if (gameState == MENU)
                    updateButton(window, mousePosition);
            }
            if (event.type == sfEvtMouseMoved) {
                mousePosition = sfMouse_getPositionRenderWindow(window);
                updateLightBtn(window, mousePosition);
            }
        }

        if (gameState != MENU && sfKeyboard_isKeyPressed(sfKeyW))
            gameState = MENU;

        if (gameState == GAME) {
            update_player(&p1, event, sfKeyQ, sfKeyD, sfKeySpace);
            update_player(&p2, event, sfKeyLeft, sfKeyRight, sfKeyReturn);

            update_bubbles(&p1);
            update_bubbles(&p2);

            update_falling_bubbles(&p1);
            update_falling_bubbles(&p2);

            send_grey_bubbles(&p1, &p2);
            send_grey_bubbles(&p2, &p1);
        }

        sfRenderWindow_clear(window, sfBlack);
        if (gameState == MENU)
            displayButton(window);
        else if (gameState == GAME) {
            // On peut dessiner un mur central ici si souhaité :
            sfRectangleShape* midWall = sfRectangleShape_create();
            sfRectangleShape_setSize(midWall, (sfVector2f) { 10, WINDOWS_HEIGHT - CEILING_Y });
            sfRectangleShape_setFillColor(midWall, sfColor_fromRGBA(255, 255, 255, 60));
            sfRectangleShape_setPosition(midWall, (sfVector2f) { (WINDOWS_WIDHT / 2) - 5, CEILING_Y });
            sfRenderWindow_drawRectangleShape(window, midWall, NULL);
            sfRectangleShape_destroy(midWall);

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

    if (gameState == MENU)
        destroyButton();
    sfRenderWindow_destroy(window);
    return 0;
}
