#include "menu.h"
#include "loading.h"
#include "audio.h"


//bouton
sfFont* font;
sfText* jouer;
sfText* joueurVsIA;
sfText* option;
sfText* quitter;
sfText* level;
sfSprite* Background;
sfTexture* textureBck;
sfSprite* Background;
sfTexture* textureBck;
sfSprite* title;
sfTexture* titleBck;

int ai_mode ;
int ai_level ;



void initButton() { //ce qui sera appellé avant le while dans le main
	font = getDefaultFont();
	jouer = sfText_create();
	option = sfText_create();
	quitter = sfText_create();
	level = sfText_create();
	//sfMusic* menuMusic = music_play("../Ressources/Sounds/JDSherbert - Ambiences Music Pack - Desert Sirocco.ogg");

	// Configuration des boutons
	sfText_setString(jouer, "JOUER");
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
	ai_mode = 0; // 0 = désactivé, 1 = activé
	


}
void updateLightBtn(sfRenderWindow* window, sfVector2i mousePosition) {
	if (mousePosition.x >= 700 && mousePosition.x <= 900) {
		if (mousePosition.y >= 300 && mousePosition.y <= 340) {
		
			sfText_setColor(jouer, sfGreen);
			sfText_setColor(option, sfWhite);
			sfText_setColor(quitter, sfWhite);
			sfText_setColor(joueurVsIA, sfWhite);

		}
		else if (mousePosition.y >= 400 && mousePosition.y <= 440) {
			
			sfText_setColor(joueurVsIA, sfCyan);
			sfText_setColor(jouer, sfWhite);
			sfText_setColor(option, sfWhite);
			sfText_setColor(quitter, sfWhite);
		}
		else if (mousePosition.y >= 500 && mousePosition.y <= 540) {
			
			sfText_setColor(option, sfMagenta);
			sfText_setColor(jouer, sfWhite);
			sfText_setColor(quitter, sfWhite);
			sfText_setColor(joueurVsIA, sfWhite);

		}
		else if (mousePosition.y >= 600 && mousePosition.y <= 640) {
			sfText_setColor(quitter, sfRed);
			sfText_setColor(jouer, sfWhite);
			sfText_setColor(option, sfWhite);
			sfText_setColor(joueurVsIA, sfWhite);
		}
	

		
	}
	else {
		sfText_setColor(jouer, sfWhite);
		sfText_setColor(option, sfWhite);
		sfText_setColor(quitter, sfWhite);
		sfText_setColor(joueurVsIA, sfWhite);
	}
}

void updateButton(sfRenderWindow* window, sfVector2i mousePosition) { // ce qui sera appelé dans le while. il s'agit des conditions relatives aux boutons appuyé avec leurs conseq
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
			ai_mode = 1;       // Mode IA activé
			ai_level = 5; // Niveau de réflexion de l'IA
			show_loading_screen(window);
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
		sfSprite_setScale(Background, (sfVector2f){5,5});

		title = sfSprite_create();
		titleBck = sfTexture_createFromFile("../Ressources/Textures/title.png", NULL);
		sfSprite_setTexture(title, titleBck, sfTrue);
		sfSprite_setScale(title, (sfVector2f) { 2, 2 });
		sfSprite_setPosition(title, (sfVector2f) { 500, 10 });





		sfRenderWindow_drawSprite(window, Background, NULL);
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

	// Flou simulé : fond semi-transparent
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
		"- ESPACE : tirer\n\n");

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
	displayButton(window);  // Dessine le menu flouté derrière
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
