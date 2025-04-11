#include "audio.h"

static sfMusic* menu_music = NULL;
static sfMusic* game_music = NULL;
static sfSoundBuffer* click_buffer = NULL;
static sfSoundBuffer* shoot_buffer = NULL;
static sfSoundBuffer* hit_buffer = NULL;
static sfSoundBuffer* match_buffer = NULL;

static sfSound* click_sound = NULL;
static sfSound* shoot_sound = NULL;
static sfSound* hit_sound = NULL;
static sfSound* match_sound = NULL;

void init_audio() {
    menu_music = sfMusic_createFromFile("../Ressources/Music/menu.ogg");
    game_music = sfMusic_createFromFile("../Ressources/Music/game.ogg");

    click_buffer = sfSoundBuffer_createFromFile("../Ressources/SoundFx/UI_button18.wav");
    shoot_buffer = sfSoundBuffer_createFromFile("../Ressources/SoundFx/shoot.wav");
    hit_buffer = sfSoundBuffer_createFromFile("../Ressources/SoundFx/hit1.wav");
    match_buffer = sfSoundBuffer_createFromFile("../Ressources/SoundFx/match.wav");

    click_sound = sfSound_create(); sfSound_setBuffer(click_sound, click_buffer);
    shoot_sound = sfSound_create(); sfSound_setBuffer(shoot_sound, shoot_buffer);
    hit_sound = sfSound_create(); sfSound_setBuffer(hit_sound, hit_buffer);
    match_sound = sfSound_create(); sfSound_setBuffer(match_sound, match_buffer);

    sfMusic_setLoop(menu_music, sfTrue);
    sfMusic_setLoop(game_music, sfTrue);
}

void destroy_audio() {
    sfMusic_destroy(menu_music);
    sfMusic_destroy(game_music);

    sfSound_destroy(click_sound);
    sfSound_destroy(shoot_sound);
    sfSound_destroy(hit_sound);
    sfSound_destroy(match_sound);

    sfSoundBuffer_destroy(click_buffer);
    sfSoundBuffer_destroy(shoot_buffer);
    sfSoundBuffer_destroy(hit_buffer);
    sfSoundBuffer_destroy(match_buffer);
}

void play_menu_music() {
    sfMusic_stop(game_music);
    sfMusic_play(menu_music);
}

void play_game_music() {
    sfMusic_stop(menu_music);
    sfMusic_play(game_music);
}

void stop_music() {
    sfMusic_stop(menu_music);
    sfMusic_stop(game_music);
}

void play_sound_click() { sfSound_play(click_sound); }
void play_sound_shoot() { sfSound_play(shoot_sound); }
void play_sound_hit() { sfSound_play(hit_sound); }
void play_sound_match() { sfSound_play(match_sound); }
