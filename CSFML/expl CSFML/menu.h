#pragma once
#include "tools.h"






void initButton();
void updateLightBtn(sfRenderWindow* window, sfVector2i mousePosition);
void updateButton(sfRenderWindow* window, sfVector2i mousePosition);
void displayButton(sfRenderWindow* window);
void destroyButton();
void show_option_panel(sfRenderWindow* window);
int is_quit_button_clicked(sfVector2i mousePos);

void show_countdown(sfRenderWindow* window); //  ajout pour le compte � rebours
int show_pause_menu(sfRenderWindow* window, sfTexture* iconTexture); // affiche le menu pause



extern int ai_mode; // 0 = d�sactiv�, 1 = activ�
extern int ai_level;  // Niveau de r�flexion de l'IA (1 � 5)