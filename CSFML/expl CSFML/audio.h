#pragma once

#include <SFML/Audio.h>

// Chargement initial
void init_audio();

// Libération mémoire
void destroy_audio();

// Contrôle musique
void play_menu_music();
void play_game_music();
void stop_music();

// Effets
void play_sound_click();
void play_sound_shoot();
void play_sound_hit();
void play_sound_match();


