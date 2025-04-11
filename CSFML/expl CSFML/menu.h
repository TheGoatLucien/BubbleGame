#pragma once
#include "tools.h"






void initButton();
void updateLightBtn(sfRenderWindow* window, sfVector2i mousePosition);
void updateButton(sfRenderWindow* window, sfVector2i mousePosition);
void displayButton(sfRenderWindow* window);
void destroyButton();

extern int ai_mode; // 0 = d�sactiv�, 1 = activ�
extern int ai_level;  // Niveau de r�flexion de l'IA (1 � 5)