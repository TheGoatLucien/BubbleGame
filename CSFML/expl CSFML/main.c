#define _CRT_SECURE_NO_WARNINGS
#include "tools.h"
#include "bubble.h"
#include "player.h"
#include "loading.h"
#include "audio.h"






int simulate_match(bubble_t* grid[ROWS][COLS], int row, int col, int color) {
    if (row < 0 || row >= ROWS || col < 0 || col >= COLS || grid[row][col] != NULL) {
        return 0; // Position invalide ou d�j� occup�e
    }

    // Simuler l'ajout de la bulle
    grid[row][col] = malloc(sizeof(bubble_t));
    grid[row][col]->color = color;

    // V�rifier les matchs
    int visited[ROWS][COLS] = { 0 };
    bubble_t* cluster[ROWS * COLS] = { 0 };
    int count = 0;

    flood_fill(grid, row, col, color, visited, cluster, &count);

    // Supprimer la bulle simul�e
    free(grid[row][col]);
    grid[row][col] = NULL;

    return count >= 3; // Retourne 1 si un match est trouv�, sinon 0
}

// Fonction pour g�rer le comportement de l'IA
void ai_play(player_t* ai_player, player_t* opponent, int level) {
    static float ai_timer = 0;
    ai_timer += getDeltaTime();
    float decision_time = 1.0f / level; // Ajuste la rapidit� de d�cision en fonction du niveau

    if (ai_timer >= decision_time) {
        ai_timer = 0;

        // S�lection d'une nouvelle bulle � tirer
        if (ai_player->current == NULL) {
            ai_player->current = ai_player->next_bubble;
            ai_player->current->active = 1;
            play_sound_shoot();
            ai_player->next_bubble = create_bubble(ai_player->launcher_pos, rand() % 4 + 1);
        }

        int best_row = -1, best_col = -1;
        int max_cluster_size = 0;

        // Exploration de toutes les positions possibles pour trouver le meilleur endroit o� tirer
        for (int row = 0; row < ROWS; row++) {
            for (int col = 0; col < COLS; col++) {
                if (opponent->grid[row][col] == NULL) { // V�rifier que la case est vide
                    int visited[ROWS][COLS] = { 0 };
                    bubble_t* cluster[ROWS * COLS] = { 0 };
                    int count = 0;

                    // Simuler l'ajout de la bulle
                    opponent->grid[row][col] = malloc(sizeof(bubble_t));
                    opponent->grid[row][col]->color = ai_player->current->color;

                    // V�rifier combien de bulles seront connect�es avec ce placement
                    flood_fill(opponent->grid, row, col, ai_player->current->color, visited, cluster, &count);

                    // Retirer la bulle simul�e
                    free(opponent->grid[row][col]);
                    opponent->grid[row][col] = NULL;

                    // S�lectionner la meilleure option (zone o� il y a le plus de connexion possible)
                    if (count > max_cluster_size) {
                        max_cluster_size = count;
                        best_row = row;
                        best_col = col;
                    }
                }
            }
        }

        // D�finition de l'angle du tir en fonction du meilleur placement trouv�
        float gridOriginX = ai_player->launcher_pos.x - (COLS * H_SPACING) / 2;

        if (best_row != -1 && best_col != -1) {
            float target_x = gridOriginX + best_col * H_SPACING + BUBBLE_RADIUS;
            float target_y = 100.0f + best_row * V_SPACING + BUBBLE_RADIUS;
            ai_player->angle = atan2(target_y - ai_player->launcher_pos.y, target_x - ai_player->launcher_pos.x);
        }
        else {
            // Strat�gie alternative : si aucun bon placement n'a �t� trouv�, utiliser un angle al�atoire
            float base_angle = ((rand() % 140) - 70) * (3.14f / 180.0f);
            float variation = rand_float(-15.0f, 15.0f) * (3.14f / 180.0f);
            ai_player->angle = base_angle + variation;
        }
    }
}





float chrono_p1 = 60.0f;
float chrono_p2 = 60.0f;

sfText* chronoText_p1 = NULL;
sfText* chronoText_p2 = NULL;

int main() {
    initTools();
    initButton();
    //
    init_audio();
    play_menu_music();


    sfRenderWindow* window = sfRenderWindow_create(
        (sfVideoMode) {
        WINDOWS_WIDHT, WINDOWS_HEIGHT, 32
    },
        "Bubble Clash", sfResize | sfClose, NULL);

    sfEvent event;
    gameState = MENU;
    int has_played_game_music = 0; //
	// Initialisation des joueurs

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
    //dessin jeu
    sfSprite* fondGaucheSprite;
    sfSprite* fondDroiteSprite;
    sfSprite* barre;
	sfSprite* barre2;
	sfTexture* barreTexture;
	sfTexture* barreTexture2;
    sfTexture* fondGaucheTexture;
    sfTexture* fondDroiteTexture;
    sfSprite* menuSprite;
    sfTexture* menuTexture;

    sfText* echapText;


    fondGaucheSprite = sfSprite_create();
    fondGaucheTexture = sfTexture_createFromFile("..//Ressources//Textures//background.png", NULL);
    fondDroiteSprite = sfSprite_create();
    fondDroiteTexture = sfTexture_createFromFile("..//Ressources//Textures//backgroundd1.png", NULL);
    barre = sfSprite_create();
    barreTexture = sfTexture_createFromFile("..//Ressources//Textures//barretimer.png", NULL);
    barre2 = sfSprite_create();
    barreTexture2 = sfTexture_createFromFile("..//Ressources//Textures//barretimer.png", NULL);
     menuSprite= sfSprite_create();
     menuTexture = sfTexture_createFromFile("..//Ressources//Textures//play.png", NULL);
     echapText = sfText_create();

    float time_elapsed = 0;
    float generation_interval = 10.0f; // Une nouvelle ligne toutes les 5 seconds
    //ini if game
	

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
        {
            //has_played_game_music = 0;

          
           
            stop_music();
            play_menu_music();
            gameState = MENU;
             
        }


        if (gameState == GAME) {
            if (!has_played_game_music) {
                stop_music();
                play_game_music();
                has_played_game_music = 1;
                // le compte � rebours
                show_countdown(window);
            }
            // Si on appuie sur �CHAP, on affiche le menu pause
            if (sfKeyboard_isKeyPressed(sfKeyEscape)) {
                sfSleep(sfMilliseconds(200)); // Anti-spam input
                sfTexture* pauseIcon = sfTexture_createFromFile("../Ressources/Textures/play1.png", NULL); // ton ic�ne
			
                int result = show_pause_menu(window, pauseIcon);
                sfTexture_destroy(pauseIcon);

                if (result == 0) { // Si "Quitter"
                    stop_music();
                    play_menu_music();
                    gameState = MENU;
                    has_played_game_music = 0;
                    // Reset jeu si besoin ici
                    continue;
                }
            }

			// G�n�ration de nouvelles lignes de bulles
             time_elapsed += getDeltaTime();

             if (ai_mode) {
                 ai_play(&p2, &p1, ai_level); // Le joueur 2 est contr�l� par l'IA
             }


     
             if (time_elapsed >= generation_interval) {
             add_random_bubble_line(&p1); // Ajoute une ligne pour le joueur 1
             add_random_bubble_line(&p2); // Ajoute une ligne pour le joueur 2
             time_elapsed = 0; // R�initialiser le timer
     
             }

            // Mise � jour des chronos
            chrono_p1 -= getDeltaTime();
            chrono_p2 -= getDeltaTime();

            if (chrono_p1 <= 0) p1.defeat = 1;
            if (chrono_p2 <= 0) p2.defeat = 1;
            


            update_player(&p1, event, sfKeyQ, sfKeyD, sfKeySpace);
            update_player(&p2, event, sfKeyLeft, sfKeyRight, sfKeyReturn);


            /*send_grey_bubbles(&p1, &p2);
            send_grey_bubbles(&p2, &p1);*/

            update_bubbles(&p1,&p2 ,&chrono_p1, &chrono_p2);
            update_bubbles(&p2,&p1, &chrono_p2, &chrono_p1);
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
        
           
           
           //

            //
            sfSprite_setTexture(fondGaucheSprite, fondGaucheTexture, sfTrue);
            sfSprite_setPosition(fondGaucheSprite, (sfVector2f) { 0, 0 });
            sfSprite_setScale(fondGaucheSprite, (sfVector2f) { 5, 3.8 });

            sfRenderWindow_drawSprite(window, fondGaucheSprite, NULL);

           //   
            sfSprite_setTexture(fondDroiteSprite, fondDroiteTexture, sfTrue);
            sfSprite_setPosition(fondDroiteSprite, (sfVector2f) { 800, 0 });
            sfSprite_setScale(fondDroiteSprite, (sfVector2f) { 5, 3.8 });

            sfRenderWindow_drawSprite(window, fondDroiteSprite, NULL);
			// Affichage de la barre de vie du joueur 1
            sfSprite_setTexture(barre, barreTexture, sfTrue);
            sfSprite_setColor(barre, sfColor_fromRGB(100, 75, 0));
            sfSprite_setPosition(barre, (sfVector2f) { 1370, 0 });
            sfRenderWindow_drawSprite(window, barre, NULL);

			// Affichage de la barre de vie du joueur 2
            sfSprite_setTexture(barre2, barreTexture2, sfTrue);
            sfSprite_setColor(barre2, sfColor_fromRGB(100, 75, 0));
            sfSprite_setPosition(barre2, (sfVector2f) { 10, 0 });
            sfRenderWindow_drawSprite(window, barre2, NULL);
            //

            sfSprite_setTexture(menuSprite, menuTexture, sfTrue);
            sfSprite_setPosition(menuSprite, (sfVector2f) { 780, 20 });
            sfSprite_setScale(menuSprite, (sfVector2f) { 0.1, 0.1 });
            sfRenderWindow_drawSprite(window, menuSprite, NULL);
             
            sfText_setString(echapText, "ECHAP");
            sfText_setFont(echapText, getDefaultFont());
            sfText_setPosition(echapText, (sfVector2f) { 780, 70 });
            sfText_setScale(echapText, (sfVector2f) { 0.5, 0.5 });
            sfText_setColor(echapText, sfBlack);
            sfRenderWindow_drawText(window, echapText, NULL);

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
                sfText_setFillColor(text, sfGreen);

                if (p1.defeat && !p2.defeat) {
                   
                    sfText_setString(text, "JOUEUR 2 A GAGNE !");
                }
                else if (p2.defeat && !p1.defeat) {
                    sfText_setString(text, "JOUEUR 1 A GAGNE !");
                }
                else {
                     sfText_setString(text, "FIN DE JEU !");
                }
                   

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
                //reset
                has_played_game_music = 0;
                stop_music();
                play_menu_music();
                gameState = MENU;
                p1 = create_player((sfVector2f) { WINDOWS_WIDHT / 4, WINDOWS_HEIGHT - 50 });
                p2 = create_player((sfVector2f) { (3 * WINDOWS_WIDHT) / 4, WINDOWS_HEIGHT - 50 });
                chrono_p1 = chrono_p2 = 60.0f;
				ai_mode = 0; // R�initialiser le mode IA
               	
                continue;
            }
        }

        sfRenderWindow_display(window);
    }

    if (gameState == MENU)
        destroyButton();
	sfSprite_destroy(fondGaucheSprite);
	sfSprite_destroy(fondDroiteSprite);
	sfSprite_destroy(barre);
	sfSprite_destroy(barre2);
	sfTexture_destroy(fondGaucheTexture);
	sfTexture_destroy(fondDroiteTexture);
	sfTexture_destroy(barreTexture);
	sfTexture_destroy(barreTexture2);


    sfText_destroy(chronoText_p1);
    sfText_destroy(chronoText_p2);
    sfRenderWindow_destroy(window);
    return 0;
}
