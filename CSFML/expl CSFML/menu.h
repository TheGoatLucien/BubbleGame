#pragma once
#include "tools.h"





void initButton();
void updateLightBtn(sfRenderWindow* window, sfVector2i mousePosition);
void updateButton(sfRenderWindow* window, sfVector2i mousePosition);
void displayButton(sfRenderWindow* window);
void destroyButton();