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

void init_audio() {  //init des Audio
    menu_music = sfMusic_createFromFile("../Ressources/Musics/Inmenu.ogg");
    game_music = sfMusic_createFromFile("../Ressources/Musics/Ingame.ogg");

    click_buffer = sfSoundBuffer_createFromFile("../Ressources/SoundsFX/UI_button18.ogg");
    shoot_buffer = sfSoundBuffer_createFromFile("../Ressources/SoundsFX/shoot.ogg");
    hit_buffer = sfSoundBuffer_createFromFile("../Ressources/SoundsFX/hit1.ogg");
    match_buffer = sfSoundBuffer_createFromFile("../Ressources/SoundsFX/match.ogg");

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

void play_menu_music() {//pour jouer la musique au menu
    sfMusic_stop(game_music);
    sfMusic_play(menu_music);
}

void play_game_music() { //pour jouer la musique dans le jeu
    sfMusic_stop(menu_music);
    sfMusic_play(game_music);
}

void stop_music() { //stop
    sfMusic_stop(menu_music);
    sfMusic_stop(game_music);
}

void play_sound_click() { sfSound_play(click_sound); } // les clicks
void play_sound_shoot() { sfSound_play(shoot_sound); } //les shoots
void play_sound_hit() { sfSound_play(hit_sound); } // les colisions
void play_sound_match() { sfSound_play(match_sound); } //sons de match
