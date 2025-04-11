#include "menu.h"



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

	sfText_setString(option, "OPTION");
	sfText_setFont(option, font);
	sfText_setCharacterSize(option, 40);
	sfText_setPosition(option, (sfVector2f) { 700, 500 });

	sfText_setString(quitter, "QUITTER");
	sfText_setFont(quitter, font);
	sfText_setCharacterSize(quitter, 40);
	sfText_setPosition(quitter, (sfVector2f) { 700, 600 });




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
 int ai_mode = 1;
int ai_level = 3;
void updateButton(sfRenderWindow* window, sfVector2i mousePosition) { // ce qui sera appelé dans le while. il s'agit des conditions relatives aux boutons appuyé avec leurs conseq
	if (mousePosition.x >= 700 && mousePosition.x <= 900) {
		if (mousePosition.y >= 300 && mousePosition.y <= 340) {
			//music_stop(menuMusic);
			//play_game(window); // Mode Jouer
			gameState = GAME;

		}
		else if (mousePosition.y >= 380 && mousePosition.y <= 440) {
			ai_mode = 1;       
			ai_level = 3;
			gameState = GAME;
		}
		else if (mousePosition.y >= 480 && mousePosition.y <= 520) {
			//music_stop(menuMusic);
			gameState = OPTION;
		}
		else if (mousePosition.y >= 580 && mousePosition.y <= 630) {
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

