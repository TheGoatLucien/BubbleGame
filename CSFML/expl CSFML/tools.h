#pragma once

#include "SFML/Graphics.h"
#include "SFML/Audio.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <windows.h>
#include <stdbool.h>

#include "menu.h"
#include "jeu.h"

#define TEXTURE_PATH "..\\Ressources\\Textures\\"
#define FONT_PATH "..\\Ressources\\Fonts\\"
#define MAP_PATH "..\\Ressources\\Maps\\"
#define TILESET_PATH "..\\Ressources\\Tilesets\\"
#define SOUND_PATH "..\\Ressources\\SoundsFX\\"
#define MUSIC_PATH "..\\Ressources\\Musics\\"

#define WINDOWS_WIDHT 1600
#define WINDOWS_HEIGHT 900

// Définition du plafond (position verticale de la barre supérieure)
#define CEILING_Y 100.0f // Descendez le plafond pour laisser plus de place


typedef struct vector4f {
    float r;
    float g;
    float b;
    float a;
} vector4f;

typedef enum {
    MENU,
    GAME,
    OPTION
} GameState;
GameState gameState;

void initTools();
sfFont* getDefaultFont();
sfVector2f vector2f(float _x, float _y);
sfVector2i vector2i(int _x, int _y);
sfVector3f vector3f(float _x, float _y, float _z);
sfFloatRect FlRect(float _left, float _top, float _width, float _height);
sfIntRect IntRect(int _left, int _top, int _width, int _height);
sfColor color(sfUint8 r, sfUint8 g, sfUint8 b, sfUint8 a);
float lerp(float v0, float v1, float t);
sfVector2f vec2f_lerp(sfVector2f v0, sfVector2f v1, float t);
sfVector3f vec3f_lerp(sfVector3f v0, sfVector3f v1, float t);
vector4f vec4f_lerp(vector4f v0, vector4f v1, float t);
sfColor color_lerp(sfColor v0, sfColor v1, float t);
void restartClock();
float getDeltaTime();
sfSprite* CreateSprite(char* _path);
sfText* CreateText(char* _path);
sfBool Circle_Collision(sfVector2f _pos1, sfVector2f _pos2, float _rayon1, float _rayon2);
sfBool Rectangle_Collision(sfFloatRect _box1, sfFloatRect _box2);
int iRand(int _min, int _max);
int rand_int(int _min, int _max);
double rand_float(float _min, float _max);
float FrameRate();
void displayFrameRate(sfRenderWindow* _window, sfText* _text);
void screenshot(sfRenderWindow* _window);
float fClamp(float value, float min, float max);
