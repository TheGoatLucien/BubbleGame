#define _CRT_SECURE_NO_WARNINGS
#include "tools.h"

sfTime sftime;
sfClock* sfclock;
sfFont* defaultFont;

#include "tools.h"

sfTime sftime;
sfClock* sfclock;
sfFont* defaultFont;


void initTools()
{
	sfclock = sfClock_create();
	srand(time(NULL));
	defaultFont = sfFont_createFromFile("../Ressources/Font/Planes_ValMore.ttf");
	//defaultFont = sfFont_createFromFile(FONT_PATH"Square.ttf");
}

sfFont* getDefaultFont()
{
	return defaultFont;
}

sfVector2f vector2f(float _x, float _y)
{
	sfVector2f tmp = { _x, _y };
	return tmp;
}

sfVector2i vector2i(int _x, int _y)
{
	sfVector2i tmp = { _x, _y };
	return tmp;
}

sfVector3f vector3f(float _x, float _y, float _z)
{
	sfVector3f tmp = { _x, _y, _z };
	return tmp;
}

sfFloatRect FlRect(float _left, float _top, float _width, float _height)
{
	sfFloatRect rectTmp = { _left, _top,  _width, _height };
	return rectTmp;
}

sfIntRect IntRect(int _left, int _top, int _width, int _height)
{
	sfIntRect rectTmp = { _left, _top,  _width, _height };
	return rectTmp;
}

sfColor color(sfUint8 r, sfUint8 g, sfUint8 b, sfUint8 a)
{
	return sfColor_fromRGBA(r, g, b, a);
}


float lerp(float v0, float v1, float t)
{
	return (1 - t) * v0 + t * v1;
}


sfVector2f vec2f_lerp(sfVector2f v0, sfVector2f v1, float t)
{
	sfVector2f tmp;
	tmp.x = (1 - t) * v0.x + t * v1.x;
	tmp.y = (1 - t) * v0.y + t * v1.y;
	return tmp;
}


sfVector3f vec3f_lerp(sfVector3f v0, sfVector3f v1, float t)
{
	sfVector3f tmp;
	tmp.x = (1 - t) * v0.x + t * v1.x;
	tmp.y = (1 - t) * v0.y + t * v1.y;
	tmp.z = (1 - t) * v0.z + t * v1.z;
	return tmp;
}


vector4f vec4f_lerp(vector4f v0, vector4f v1, float t)
{
	vector4f tmp;
	tmp.r = (1 - t) * v0.r + t * v1.r;
	tmp.g = (1 - t) * v0.g + t * v1.g;
	tmp.b = (1 - t) * v0.b + t * v1.b;
	tmp.a = (1 - t) * v0.a + t * v1.a;
	return tmp;
}


sfColor color_lerp(sfColor v0, sfColor v1, float t)
{
	sfColor tmp;
	tmp.r = (1 - t) * v0.r + t * v1.r;
	tmp.g = (1 - t) * v0.g + t * v1.g;
	tmp.b = (1 - t) * v0.b + t * v1.b;
	tmp.a = (1 - t) * v0.a + t * v1.a;
	return tmp;
}

void restartClock()
{
	sftime = sfClock_restart(sfclock);
}

float getDeltaTime()
{
	float tmp = sfTime_asSeconds(sftime);
	if (tmp > 0.1) tmp = 0.0f;
	return tmp;
}

sfSprite* CreateSprite(char* _path)
{
	sfTexture* tmp = sfTexture_createFromFile(_path, NULL);
	sfSprite* spTmp = sfSprite_create();
	sfSprite_setTexture(spTmp, tmp, sfTrue);
	return spTmp;
}

sfText* CreateText(char* _path)
{
	sfFont* tmp = sfFont_createFromFile(_path);
	sfText* txtTmp = sfText_create();
	sfText_setFont(txtTmp, tmp);
	return txtTmp;
}

sfBool Circle_Collision(sfVector2f _pos1, sfVector2f _pos2, float _rayon1, float _rayon2)
{
	// Version non optimisé
	// if (sqrt(pow(_pos1.x - _pos2.x, 2) + pow(_pos1.y - _pos2.y, 2)) < _rayon1 + _rayon2) 

	// Version Optimisé
	if ((_pos1.x - _pos2.x) * (_pos1.x - _pos2.x) + (_pos1.y - _pos2.y) * (_pos1.y - _pos2.y) < (_rayon1 + _rayon2) * (_rayon1 + _rayon2))
	{
		return sfTrue;
	}
	else return sfFalse;
}

sfBool Rectangle_Collision(sfFloatRect _box1, sfFloatRect _box2)
{
	if ((_box2.left >= _box1.left + _box1.width)      // trop à droite
		|| (_box2.left + _box2.width <= _box1.left) // trop à gauche
		|| (_box2.top >= _box1.top + _box1.height) // trop en bas
		|| (_box2.top + _box2.height <= _box1.top))  // trop en haut
		return sfFalse;
	else
		return sfTrue;
}

int iRand(int _min, int _max)
{
	if (_max > _min)
		return rand() % (_max - _min + 1) + _min;
	else
		return _min;
}

int rand_int(int _min, int _max)
{
	return rand() % (_max - _min) + _min;
}

double rand_float(float _min, float _max)
{
	return ((rand() / (double)RAND_MAX) * ((double)_max - (double)_min) + (double)_min);
}

float FrameRate()
{
	return 1.0f / getDeltaTime();
}

void displayFrameRate(sfRenderWindow* _window, sfText* _text)
{
	static float displayTimer = 0.0f;
	static char frameRate[25];

	displayTimer += getDeltaTime();

	if (displayTimer > 0.1f)
	{
		sprintf_s(frameRate, 25, "FPS : %.2f", FrameRate());
		displayTimer = 0.0f;
	}

	sfText_setString(_text, frameRate);
	sfRenderWindow_drawText(_window, _text, NULL);
}
int fClamp(int value, int min, int max) {
	if (value < min) return min;
	if (value > max) return max;
	return value;
}

void screenshot(sfRenderWindow* _window)
{
	char hours[9] = { 0 }, date[9] = { 0 }, tmph[7] = { 0 }, tmpd[7] = { 0 };
	_strtime(hours);
	_strdate(date);
	sfVector2u windowSize;


	windowSize = sfRenderWindow_getSize(_window);
	sfTexture* texture = sfTexture_create(windowSize.x, windowSize.y);
	sfTexture_updateFromRenderWindow(texture, _window, 0, 0);
	sfImage* screenshot = sfTexture_copyToImage(texture);
	char filename[100];
	int j = 0;
	for (int i = 0; i < 9; i++) if (hours[i] != ':') tmph[j++] = hours[i];
	j = 0;
	for (int i = 0; i < 9; i++) if (date[i] != '/') tmpd[j++] = date[i];

	sprintf(filename, "../Ressources/Screenshots/Screenshot-%s-%s.jpg", tmpd, tmph);
	sfImage_saveToFile(screenshot, filename);
}