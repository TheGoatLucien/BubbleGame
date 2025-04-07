#include "menu.h"



//bouton
sfFont* font;
sfText* jouer;
sfText* option;
sfText* quitter;
sfText* level;


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
	sfText_setPosition(jouer, (sfVector2f) { 300, 150 });

	sfText_setString(option, "OPTION");
	sfText_setFont(option, font);
	sfText_setCharacterSize(option, 40);
	sfText_setPosition(option, (sfVector2f) { 300, 250 });

	sfText_setString(quitter, "QUITTER");
	sfText_setFont(quitter, font);
	sfText_setCharacterSize(quitter, 40);
	sfText_setPosition(quitter, (sfVector2f) { 300, 350 });
}
void updateLightBtn(sfRenderWindow* window, sfVector2i mousePosition) {
	if (mousePosition.x >= 300 && mousePosition.x <= 500) {
		if (mousePosition.y >= 150 && mousePosition.y <= 190) {
		
			sfText_setColor(jouer, sfGreen);
			sfText_setColor(option, sfWhite);
			sfText_setColor(quitter, sfWhite);

		}
		else if (mousePosition.y >= 250 && mousePosition.y <= 290) {
			
			sfText_setColor(option, sfMagenta);
			sfText_setColor(jouer, sfWhite);
			sfText_setColor(quitter, sfWhite);

		}
		else if (mousePosition.y >= 350 && mousePosition.y <= 390) {
			sfText_setColor(quitter, sfRed);
			sfText_setColor(jouer, sfWhite);
			sfText_setColor(option, sfWhite);
		}
		
	}
	else {
		sfText_setColor(jouer, sfWhite);
		sfText_setColor(option, sfWhite);
		sfText_setColor(quitter, sfWhite);
	}
}
void updateButton(sfRenderWindow* window, sfVector2i mousePosition) { // ce qui sera appelé dans le while. il s'agit des conditions relatives aux boutons appuyé avec leurs conseq
	if (mousePosition.x >= 300 && mousePosition.x <= 500) {
		if (mousePosition.y >= 150 && mousePosition.y <= 190) {
			//music_stop(menuMusic);
			//play_game(window); // Mode Jouer
			gameState = GAME;

		}
		else if (mousePosition.y >= 250 && mousePosition.y <= 290) {
			//music_stop(menuMusic);
			gameState = OPTION;
		}
		else if (mousePosition.y >= 350 && mousePosition.y <= 390) {
			sfRenderWindow_close(window);
		}
	}


}
void displayButton(sfRenderWindow* window) { // display dans le displqy window
	if (gameState == MENU) {
		sfRenderWindow_drawText(window, jouer, NULL);
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
	sfText_destroy(option);
	sfText_destroy(quitter);
	sfFont_destroy(font);
}

