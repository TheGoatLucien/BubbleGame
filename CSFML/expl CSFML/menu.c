#define _CRT_SECURE_NO_WARNINGS
#include "menu.h"
#include "loading.h"
#include "audio.h"

#define STAR_COUNT 100
static sfCircleShape* stars[STAR_COUNT];

//boutton
sfFont* font;
sfText* jouer;
sfText* joueurVsIA;
sfText* option;
sfText* quitter;
sfText* level;
sfSprite* Background;
sfTexture* textureBck;
sfSprite* title;
sfTexture* titleBck;

int ai_mode ;
int ai_level ;



void initButton() { //ce qui sera appell� avant le while dans le main
	font = getDefaultFont();
	jouer = sfText_create();
	option = sfText_create();
	quitter = sfText_create();
	level = sfText_create();
	//sfMusic* menuMusic = music_play("../Ressources/Sounds/JDSherbert - Ambiences Music Pack - Desert Sirocco.ogg");

	// Configuration des boutons
	sfText_setString(jouer, "JOUER 1V1");
	sfText_setFont(jouer, font);
	sfText_setCharacterSize(jouer, 40);
	sfText_setPosition(jouer, (sfVector2f) { 700, 300 });


	joueurVsIA = sfText_create();
	sfText_setString(joueurVsIA, "JOUEUR VS IA");
	sfText_setFont(joueurVsIA, font);
	sfText_setCharacterSize(joueurVsIA, 40);
	sfText_setPosition(joueurVsIA, (sfVector2f) { 700, 400 });  

	sfText_setString(option, "CONTROLES");
	sfText_setFont(option, font);
	sfText_setCharacterSize(option, 40);
	sfText_setPosition(option, (sfVector2f) { 700, 500 });

	sfText_setString(quitter, "QUITTER");
	sfText_setFont(quitter, font);
	sfText_setCharacterSize(quitter, 40);
	sfText_setPosition(quitter, (sfVector2f) { 700, 600 });

	//
	ai_mode = 0; // 0 = d�sactiv�, 1 = activ�

	// 
	for (int i = 0; i < STAR_COUNT; i++) {
		stars[i] = sfCircleShape_create();
		sfCircleShape_setRadius(stars[i], rand_float(1.0f, 2.5f));
		sfCircleShape_setFillColor(stars[i], sfWhite);
		float x = rand_float(0, WINDOWS_WIDHT);
		float y = rand_float(0, WINDOWS_HEIGHT);
		sfCircleShape_setPosition(stars[i], (sfVector2f) { x, y });
	}

	


}
void updateLightBtn(sfRenderWindow* window, sfVector2i mousePosition) {
	if (mousePosition.x >= 700 && mousePosition.x <= 900) {
		if (mousePosition.y >= 300 && mousePosition.y <= 340) {
		
			sfText_setColor(jouer, sfGreen);
			sfText_setScale(jouer, (sfVector2f) { 1.2f, 1.2f });
			sfText_setScale(option, (sfVector2f) { 1.0f, 1.0f });
			sfText_setScale(quitter, (sfVector2f) { 1.0f, 1.0f });
			sfText_setScale(joueurVsIA, (sfVector2f) { 1.0f, 1.0f });



			sfText_setColor(option, sfWhite);
			sfText_setColor(quitter, sfWhite);
			sfText_setColor(joueurVsIA, sfWhite);

		}
		else if (mousePosition.y >= 400 && mousePosition.y <= 440) {
			
			sfText_setColor(joueurVsIA, sfCyan);
			sfText_setScale(joueurVsIA, (sfVector2f) { 1.2f, 1.2f });
			sfText_setScale(jouer, (sfVector2f) { 1.0f, 1.0f });
			sfText_setScale(option, (sfVector2f) { 1.0f, 1.0f });
			sfText_setScale(quitter, (sfVector2f) { 1.0f, 1.0f });

			sfText_setColor(jouer, sfWhite);
			sfText_setColor(option, sfWhite);
			sfText_setColor(quitter, sfWhite);
		}
		else if (mousePosition.y >= 500 && mousePosition.y <= 540) {
			
			sfText_setColor(option, sfMagenta);
			sfText_setScale(option, (sfVector2f) { 1.2f, 1.2f });
			sfText_setScale(jouer, (sfVector2f) { 1.0f, 1.0f });
			sfText_setScale(joueurVsIA, (sfVector2f) { 1.0f, 1.0f });
			sfText_setScale(quitter, (sfVector2f) { 1.0f, 1.0f });


			sfText_setColor(jouer, sfWhite);
			sfText_setColor(quitter, sfWhite);
			sfText_setColor(joueurVsIA, sfWhite);

		}
		else if (mousePosition.y >= 600 && mousePosition.y <= 640) {
			sfText_setColor(quitter, sfRed);
			sfText_setScale(quitter, (sfVector2f) { 1.2f, 1.2f });
			sfText_setScale(jouer, (sfVector2f) { 1.0f, 1.0f });
			sfText_setScale(option, (sfVector2f) { 1.0f, 1.0f });
			sfText_setScale(joueurVsIA, (sfVector2f) { 1.0f, 1.0f });

			sfText_setColor(jouer, sfWhite);
			sfText_setColor(option, sfWhite);
			sfText_setColor(joueurVsIA, sfWhite);
		}
	

		
	}
	else {
		sfText_setScale(jouer, (sfVector2f) { 1.0f, 1.0f });
		sfText_setScale(joueurVsIA, (sfVector2f) { 1.0f, 1.0f });
		sfText_setScale(option, (sfVector2f) { 1.0f, 1.0f });
		sfText_setScale(quitter, (sfVector2f) { 1.0f, 1.0f });

		sfText_setColor(jouer, sfWhite);
		sfText_setColor(option, sfWhite);
		sfText_setColor(quitter, sfWhite);
		sfText_setColor(joueurVsIA, sfWhite);
	}
}

void updateButton(sfRenderWindow* window, sfVector2i mousePosition) { // ce qui sera appel� dans le while. il s'agit des conditions relatives aux boutons appuy� avec leurs conseq
	if (mousePosition.x >= 700 && mousePosition.x <= 900) {
		if (mousePosition.y >= 300 && mousePosition.y <= 340) {
			//music_stop(menuMusic);
			//play_game(window); // Mode Jouer
			//play_sound_click();
			play_sound_click();
			sfSleep(sfMilliseconds(100)); // Petite pause pour entendre le clic
			show_loading_screen(window);
			
			gameState = GAME;

		}
		else if (mousePosition.y >= 380 && mousePosition.y <= 440) {
			play_sound_click();
			ai_mode = 1;       // Mode IA activ�
		
			show_loading_screen(window);
			ai_level = ask_ia_level(window); //  demande du niveau de l'IA ici
			sfSleep(sfMilliseconds(100)); // Petite pause pour entendre le clic
			gameState = GAME;
		}
		else if (mousePosition.y >= 500 && mousePosition.y <= 540) {
			play_sound_click();
			//music_stop(menuMusic);
			show_option_panel(window);  //
		}
		else if (mousePosition.y >= 600 && mousePosition.y <= 640) {
			play_sound_click();
			sfRenderWindow_close(window);
		}
	}


}
void displayButton(sfRenderWindow* window) { // display dans le displqy window
	if (gameState == MENU) {
		Background = sfSprite_create();
		textureBck = sfTexture_createFromFile("../Ressources/Textures/titlescreen.png",NULL);
		sfSprite_setTexture(Background, textureBck,sfTrue);
		sfSprite_setScale(Background, (sfVector2f){6,6});

		title = sfSprite_create();
		titleBck = sfTexture_createFromFile("../Ressources/Textures/title.png", NULL);
		sfSprite_setTexture(title, titleBck, sfTrue);
		sfSprite_setScale(title, (sfVector2f) { 4, 4 });
		sfSprite_setPosition(title, (sfVector2f) { 630, 20 });
	
		sfRenderWindow_drawSprite(window, Background, NULL);

		// Effet �toiles anim�es en fond
		for (int i = 0; i < STAR_COUNT; i++) {
			sfVector2f pos = sfCircleShape_getPosition(stars[i]);
			pos.y += 0.2f; // vitesse lente vers le bas
			if (pos.y > WINDOWS_HEIGHT) pos.y = 0;
			sfCircleShape_setPosition(stars[i], pos);
			sfRenderWindow_drawCircleShape(window, stars[i], NULL);
		}

		

		float time = getDeltaTime();
		static float scale_anim = 1.0f;
		static int growing = 1;
		if (growing) {
			scale_anim += time * 0.5f;
			if (scale_anim >= 1.6f) growing = 0;
		}
		else {
			scale_anim -= time * 0.5f;
			if (scale_anim <= 0.95f) growing = 1;
		}
		sfSprite_setScale(title, (sfVector2f) { scale_anim, scale_anim });
		sfRenderWindow_drawSprite(window, title, NULL);

		sfRenderWindow_drawText(window, jouer, NULL);
		sfRenderWindow_drawText(window, joueurVsIA, NULL);
		sfRenderWindow_drawText(window, level, NULL);
		sfRenderWindow_drawText(window, option, NULL);
		sfRenderWindow_drawText(window, quitter, NULL);
	}
	if (gameState == 1) { 
		/*sfRenderWindow_drawRectangleShape(window, buttonPause.buttonShape, NULL);*/
	}


}
void destroyButton() { // apres la boucle while pour liberer l'espace
	for (int i = 0; i < STAR_COUNT; i++) {
		if (stars[i]) sfCircleShape_destroy(stars[i]);
	}

	sfText_destroy(jouer);
	sfText_destroy(level);
	sfText_destroy(joueurVsIA);
	sfText_destroy(option);
	sfText_destroy(quitter);
	sfFont_destroy(font);
	sfSprite_destroy(Background);
	sfTexture_destroy(textureBck);
}

void show_option_panel(sfRenderWindow* window) {
	sfFont* font = getDefaultFont();

	// Flou simul� : fond semi-transparent
	sfRectangleShape* overlay = sfRectangleShape_create();
	sfRectangleShape_setSize(overlay, (sfVector2f) { WINDOWS_WIDHT, WINDOWS_HEIGHT });
	sfRectangleShape_setFillColor(overlay, sfColor_fromRGBA(0, 0, 0, 150));

	// Grand rectangle blanc central
	sfRectangleShape* panel = sfRectangleShape_create();
	sfVector2f size = { 800, 600 };
	sfVector2f pos = { (WINDOWS_WIDHT - size.x) / 2, (WINDOWS_HEIGHT - size.y) / 2 };
	sfRectangleShape_setSize(panel, size);
	sfRectangleShape_setPosition(panel, pos);
	sfRectangleShape_setFillColor(panel, sfWhite);

	// Texte des commandes
	sfText* controls = sfText_create();
	sfText_setFont(controls, font);
	sfText_setCharacterSize(controls,24);
	sfText_setFillColor(controls, sfBlack);
	sfText_setString(controls,
		"COMMANDES - JEU SUR CLAVIER\n\n"
		"MODE JOUEUR VS JOUEUR:\n"
		"- JOUEUR 1\n"
		"- Q / D : viser\n"
		"- ESPACE : tirer\n\n"

		"- JOUEUR 2\n"
		"- GAUCHE / DROIT : viser\n"
		"- ENTRER : tirer\n\n"

		"JOUEUR  VS IA:\n"
		"- Q / D  : viser\n"
		"- ESPACE : tirer\n\n\n"
	
		"- ECHAP : MENU PAUSE\n\n");





	sfText_setPosition(controls, (sfVector2f) { pos.x + 30, pos.y + 40 });

	// Bouton "Quitter"
	sfRectangleShape* quitBtn = sfRectangleShape_create();
	sfVector2f qsize = { 200, 50 };
	sfVector2f qpos = { pos.x + (size.x - qsize.x) / 2, pos.y + size.y - 70 };
	sfRectangleShape_setSize(quitBtn, qsize);
	sfRectangleShape_setPosition(quitBtn, qpos);
	sfRectangleShape_setFillColor(quitBtn, sfColor_fromRGB(180, 50, 50));

	sfText* quitText = sfText_create();
	sfText_setFont(quitText, font);
	sfText_setCharacterSize(quitText,24);
	sfText_setString(quitText, "QUITTER");
	sfText_setFillColor(quitText, sfWhite);
	sfFloatRect tb = sfText_getLocalBounds(quitText);
	sfText_setPosition(quitText, (sfVector2f) { qpos.x + (qsize.x - tb.width) / 2, qpos.y + 10 });

	sfRenderWindow_clear(window, sfBlack);
	displayButton(window);  // Dessine le menu flout� derri�re
	sfRenderWindow_drawRectangleShape(window, overlay, NULL);
	sfRenderWindow_drawRectangleShape(window, panel, NULL);
	sfRenderWindow_drawText(window, controls, NULL);
	sfRenderWindow_drawRectangleShape(window, quitBtn, NULL);
	sfRenderWindow_drawText(window, quitText, NULL);
	sfRenderWindow_display(window);

	// Boucle d'attente d'interaction
	sfEvent event;
	while (sfRenderWindow_waitEvent(window, &event)) {
		if (event.type == sfEvtClosed) {
			sfRenderWindow_close(window);
			break;
		}

		if (event.type == sfEvtMouseButtonPressed) {
			sfVector2i mp = sfMouse_getPositionRenderWindow(window);
			if (is_quit_button_clicked(mp)) {
				break; // Sortir du panneau d'aide
			}
		}
	}

	// Nettoyage
	sfRectangleShape_destroy(overlay);
	sfRectangleShape_destroy(panel);
	sfRectangleShape_destroy(quitBtn);
	sfText_destroy(controls);
	sfText_destroy(quitText);
}
int is_quit_button_clicked(sfVector2i mousePos) {
	int panelX = (WINDOWS_WIDHT - 800) / 2;
	int panelY = (WINDOWS_HEIGHT - 600) / 2;

	int quitX = panelX + (800 - 200) / 2;
	int quitY = panelY + 600 - 70;

	return (mousePos.x >= quitX && mousePos.x <= quitX + 200 &&
		mousePos.y >= quitY && mousePos.y <= quitY + 50);
}


void show_countdown(sfRenderWindow* window) { //decompte
	sfFont* font = getDefaultFont();
	sfText* text = sfText_create();
	sfText_setFont(text, font);
	sfText_setCharacterSize(text, 100);
	sfText_setFillColor(text, sfWhite);

	sfVector2f pos = { WINDOWS_WIDHT / 2 - 100, WINDOWS_HEIGHT / 2 - 100 };
	sfText_setPosition(text, pos);

	for (int i = 3; i >= 1; i--) {
		char buffer[2];
		sprintf(buffer, "%d", i);
		sfText_setString(text, buffer);
		sfRenderWindow_clear(window, sfBlack);
		sfRenderWindow_drawText(window, text, NULL);
		sfRenderWindow_display(window);
		sfSleep(sfSeconds(1));
	}
	 pos =(sfVector2f) { WINDOWS_WIDHT / 2 - 200, WINDOWS_HEIGHT / 2 - 100 };
	sfText_setPosition(text, pos);
	sfText_setString(text, "CEST PARTI !");
	sfRenderWindow_clear(window, sfBlack);
	sfRenderWindow_drawText(window, text, NULL);
	sfRenderWindow_display(window);
	
	sfSleep(sfSeconds(1));

	sfText_destroy(text);
}



int show_pause_menu(sfRenderWindow* window, sfTexture* iconTexture) { //menu pause
	sfFont* font = getDefaultFont();

	// Fond semi-transparent
	sfRectangleShape* overlay = sfRectangleShape_create();
	sfRectangleShape_setSize(overlay, (sfVector2f) { WINDOWS_WIDHT, WINDOWS_HEIGHT });
	sfRectangleShape_setFillColor(overlay, sfColor_fromRGBA(0, 0, 0, 150));

	// Panneau central blanc
	sfRectangleShape* panel = sfRectangleShape_create();
	sfVector2f size = { 600, 300 };
	sfVector2f pos = { (WINDOWS_WIDHT - size.x) / 2, (WINDOWS_HEIGHT - size.y) / 2 };
	sfRectangleShape_setSize(panel, size);
	sfRectangleShape_setPosition(panel, pos);
	sfRectangleShape_setFillColor(panel, sfWhite);

	// Ic�ne pause (centr�e en haut du panneau)
	sfSprite* icon = sfSprite_create();
	sfSprite_setTexture(icon, iconTexture, sfTrue);
	sfSprite_setScale(icon, (sfVector2f) { 0.4f, 0.4f });
	sfSprite_setPosition(icon, (sfVector2f) { pos.x + size.x / 2 - 64, pos.y - 64 });

	// Texte "Continuer"
	sfText* continuer = sfText_create();
	sfText_setFont(continuer, font);
	sfText_setString(continuer, "Continuer");
	sfText_setCharacterSize(continuer,36);
	sfText_setFillColor(continuer, sfBlack);
	sfText_setPosition(continuer, (sfVector2f) { pos.x + 200, pos.y + 100 });

	// Texte "Quitter"
	sfText* quitter = sfText_create();
	sfText_setFont(quitter, font);
	sfText_setString(quitter, "Quitter");
	sfText_setCharacterSize(quitter,36);
	sfText_setFillColor(quitter, sfBlack);
	sfText_setPosition(quitter, (sfVector2f) { pos.x + 215, pos.y + 170 });

	while (sfRenderWindow_isOpen(window)) {
		sfEvent event;
		sfFloatRect contBounds = sfText_getGlobalBounds(continuer);
		sfFloatRect quitBounds = sfText_getGlobalBounds(quitter);
		while (sfRenderWindow_pollEvent(window, &event)) {
			if (event.type == sfEvtClosed) {
				sfRenderWindow_close(window);
				return 0;
			}

			if (event.type == sfEvtMouseButtonPressed) {
				sfVector2f mouse = { event.mouseButton.x, event.mouseButton.y };

				if (sfFloatRect_contains(&contBounds, mouse.x, mouse.y)) {
					sfText_destroy(continuer);
					sfText_destroy(quitter);
					sfSprite_destroy(icon);
					sfRectangleShape_destroy(panel);
					sfRectangleShape_destroy(overlay);
					return 1;
				}

				if (sfFloatRect_contains(&quitBounds, mouse.x, mouse.y)) {
					sfText_destroy(continuer);
					sfText_destroy(quitter);
					sfSprite_destroy(icon);
					sfRectangleShape_destroy(panel);
					sfRectangleShape_destroy(overlay);
					return 0;
				}
			}

			if (event.type == sfEvtMouseMoved) {
				sfVector2i mouse = { event.mouseMove.x, event.mouseMove.y };
				if (sfFloatRect_contains(&contBounds, mouse.x, mouse.y)) {
					sfText_setFillColor(continuer, sfGreen);
					sfText_setScale(continuer, (sfVector2f) { 1.1f, 1.1f });
				}
				else {
					sfText_setFillColor(continuer, sfBlack);
					sfText_setScale(continuer, (sfVector2f) { 1.f, 1.f });
				}

				if (sfFloatRect_contains(&quitBounds, mouse.x, mouse.y)) {
					sfText_setFillColor(quitter, sfRed);
					sfText_setScale(quitter, (sfVector2f) { 1.1f, 1.1f });
				}
				else {
					sfText_setFillColor(quitter, sfBlack);
					sfText_setScale(quitter, (sfVector2f) { 1.f, 1.f });
				}
			}
		}

		sfRenderWindow_clear(window, sfBlack);
		sfRenderWindow_drawRectangleShape(window, overlay, NULL);
		sfRenderWindow_drawRectangleShape(window, panel, NULL);
		sfRenderWindow_drawSprite(window, icon, NULL);
		sfRenderWindow_drawText(window, continuer, NULL);
		sfRenderWindow_drawText(window, quitter, NULL);
		sfRenderWindow_display(window);
	}

	return 0;
}

