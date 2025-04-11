#define _CRT_SECURE_NO_WARNINGS
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

float chrono_p1 = 60.0f;
float chrono_p2 = 60.0f;

sfText* chronoText_p1 = NULL;
sfText* chronoText_p2 = NULL;

int main() {
    initTools();
    initButton();

    sfRenderWindow* window = sfRenderWindow_create(
        (sfVideoMode) {
        WINDOWS_WIDHT, WINDOWS_HEIGHT, 32
    },
        "Bubble Clash", sfResize | sfClose, NULL);

    sfEvent event;
    gameState = MENU;

    player_t p1 = create_player((sfVector2f) { WINDOWS_WIDHT / 4, WINDOWS_HEIGHT - 50 });
    player_t p2 = create_player((sfVector2f) { (3 * WINDOWS_WIDHT) / 4, WINDOWS_HEIGHT - 50 });
    sfVector2i mousePosition;

    // Chrono visuel init
    chronoText_p1 = sfText_create();
    chronoText_p2 = sfText_create();
    sfFont* font = getDefaultFont();
    sfText_setFont(chronoText_p1, font);
    sfText_setFont(chronoText_p2, font);
    sfText_setCharacterSize(chronoText_p1, 26);
    sfText_setCharacterSize(chronoText_p2, 26);
    sfText_setFillColor(chronoText_p1, sfWhite);
    sfText_setFillColor(chronoText_p2, sfWhite);

    float time_elapsed = 0;
    float generation_interval = 10.0f; // Une nouvelle ligne toutes les 5 seconds


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
			// Génération de nouvelles lignes de bulles
             time_elapsed += getDeltaTime();

     
             if (time_elapsed >= generation_interval) {
         add_random_bubble_line(&p1); // Ajoute une ligne pour le joueur 1
         add_random_bubble_line(&p2); // Ajoute une ligne pour le joueur 2
         time_elapsed = 0; // Réinitialiser le timer
     
             }

            // Mise à jour des chronos
            chrono_p1 -= getDeltaTime();
            chrono_p2 -= getDeltaTime();

            if (chrono_p1 <= 0) p1.defeat = 1;
            if (chrono_p2 <= 0) p2.defeat = 1;
            


            update_player(&p1, event, sfKeyQ, sfKeyD, sfKeySpace);
            update_player(&p2, event, sfKeyLeft, sfKeyRight, sfKeyReturn);


            send_grey_bubbles(&p1, &p2);
            send_grey_bubbles(&p2, &p1);

            update_bubbles(&p1, &chrono_p1);
            update_bubbles(&p2, &chrono_p2);
			update_bubble_animations(&p1, getDeltaTime());
			update_bubble_animations(&p2, getDeltaTime());

            update_falling_bubbles(&p1);
            update_falling_bubbles(&p2);

        }

        sfRenderWindow_clear(window, sfBlack);

        if (gameState == MENU) {
            displayButton(window);
        }
        else if (gameState == GAME) {
            //dessins du jeu
            sfSprite* fondGaucheSprite;
            fondGaucheSprite = sfSprite_create();
            sfTexture* fondGaucheTexture;
            fondGaucheTexture = sfTexture_createFromFile("..//Ressources//Textures//background.png", NULL);
            if (!fondGaucheTexture) {
                printf("Erreur : Impossible de charger la texture fg.png\n");
            }

            sfSprite_setTexture(fondGaucheSprite, fondGaucheTexture, sfTrue);
            sfSprite_setPosition(fondGaucheSprite, (sfVector2f) { 0, 0 });
            sfSprite_setScale(fondGaucheSprite, (sfVector2f) { 5, 3.8 });

            sfRenderWindow_drawSprite(window, fondGaucheSprite, NULL);

            sfSprite* fondDroiteSprite = sfSprite_create();
            sfTexture* fondDroiteTexture = sfTexture_createFromFile("..//Ressources//Textures//backgroundd1.png", NULL);
            sfSprite_setTexture(fondDroiteSprite, fondDroiteTexture, sfTrue);
            sfSprite_setPosition(fondDroiteSprite, (sfVector2f) { 800, 0 });
            sfSprite_setScale(fondDroiteSprite, (sfVector2f) { 5, 3.8 });

            sfRenderWindow_drawSprite(window, fondDroiteSprite, NULL);

            sfSprite* barre = sfSprite_create();
            sfTexture* barreTexture = sfTexture_createFromFile("..//Ressources//Textures//barretimer.png", NULL);
            sfSprite_setTexture(barre, barreTexture, sfTrue);
            sfSprite_setColor(barre, sfColor_fromRGB(100, 75, 0));
            sfSprite_setPosition(barre, (sfVector2f) { 1370, 0 });
            sfRenderWindow_drawSprite(window, barre, NULL);

            sfSprite* barre2 = sfSprite_create();
            sfTexture* barreTexture2 = sfTexture_createFromFile("..//Ressources//Textures//barretimer.png", NULL);
            sfSprite_setTexture(barre2, barreTexture2, sfTrue);
            sfSprite_setColor(barre2, sfColor_fromRGB(100, 75, 0));
            sfSprite_setPosition(barre2, (sfVector2f) { 10, 0 });
            sfRenderWindow_drawSprite(window, barre2, NULL);

            //
            draw_bubble_animations(&p1, window);
            draw_bubble_animations(&p2, window);

            //
            draw_player(&p1, window);
            draw_player(&p2, window);
            //affichage visuel timer
            update_bonus_animation(&p1, window);
            update_bonus_animation(&p2, window);
            // Affichage chrono joueur 1
            char str_p1[64];
            sprintf(str_p1, "Temps P1: %.1f s", chrono_p1);
            sfText_setString(chronoText_p1, str_p1);
            sfText_setPosition(chronoText_p1, (sfVector2f) { 20, 20 });
			sfText_setColor(chronoText_p1, sfBlue);
            sfRenderWindow_drawText(window, chronoText_p1, NULL);

            // Affichage chrono joueur 2
            char str_p2[64];
            sprintf(str_p2, "Temps P2: %.1f s", chrono_p2);
            sfText_setString(chronoText_p2, str_p2);
            sfText_setPosition(chronoText_p2, (sfVector2f) { WINDOWS_WIDHT - 220, 20 });
			sfText_setColor(chronoText_p2, sfRed);
            sfRenderWindow_drawText(window, chronoText_p2, NULL);

            // Condition de fin
            if (p1.defeat || p2.defeat) {
                sfText* text = sfText_create();
                sfText_setFont(text, font);
                sfText_setCharacterSize(text, 60);
                sfText_setFillColor(text, sfRed);

                if (p1.defeat && !p2.defeat)
                    sfText_setString(text, "JOUEUR 2 A GAGNE !");
                else if (p2.defeat && !p1.defeat)
                    sfText_setString(text, "JOUEUR 1 A GAGNE !");
                else
                    sfText_setString(text, "FIN DE JEU !");

                sfFloatRect bounds = sfText_getLocalBounds(text);
                sfVector2f pos = {
                    (WINDOWS_WIDHT - bounds.width) / 2,
                    (WINDOWS_HEIGHT - bounds.height) / 2
                };
                sfText_setPosition(text, pos);
                sfRenderWindow_drawText(window, text, NULL);
                sfRenderWindow_display(window);

                sfSleep(sfSeconds(4));
                sfText_destroy(text);

                gameState = MENU;
                p1 = create_player((sfVector2f) { WINDOWS_WIDHT / 4, WINDOWS_HEIGHT - 50 });
                p2 = create_player((sfVector2f) { (3 * WINDOWS_WIDHT) / 4, WINDOWS_HEIGHT - 50 });
                chrono_p1 = chrono_p2 = 60.0f;
                continue;
            }
        }

        sfRenderWindow_display(window);
    }

    if (gameState == MENU)
        destroyButton();

    sfText_destroy(chronoText_p1);
    sfText_destroy(chronoText_p2);
    sfRenderWindow_destroy(window);
    return 0;
}
