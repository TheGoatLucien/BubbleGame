#pragma once

#include "tools.h"
#include <SFML/Graphics.h>
#include <SFML/System.h>

// Fonction � appeler depuis le main quand on passe du menu au jeu
void show_loading_screen(sfRenderWindow* window);
int ask_ia_level(sfRenderWindow* window);
// LOADING_SCREEN_H
