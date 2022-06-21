#pragma once

#include <SDL2/SDL_mixer.h>
#include <map>

#define ATREIDES_MUSIC_PATH "/atreides-background.wav"
#define HARKONNEN_MUSIC_PATH "/harkonnen-background.wav"
#define ORDOS_MUSIC_PATH "/ordos-background.wav"

#define GAME_START_PATH "/sounds/game-start.wav"
#define GUN1_PATH "/sounds/20MMGUN1.WAV"
#define GUN2_PATH "/sounds/MGUN2.WAV"
#define MISSILE_PATH "/sounds/MISSLE1.WAV"
#define EXPLHG1_PATH "/sounds/EXPLHG1.WAV"
#define ROCKET1_PATH "/sounds/ROCKET1.WAV"
#define EXPLSML2_PATH "/sounds/EXPLSML2.WAV"

enum sfx_t {
    GAME_START,
    GUN_1,
    GUN_2,
    MISSILE,
    EXPLHG1,
    ROCKET1,
    EXPLSML2
};

enum music_t {
    ATREIDES_MUSIC,
    HARKONNEN_MUSIC,
    ORDOS_MUSIC
};

class AudioPlayer {
    private:
    std::map<sfx_t,Mix_Chunk*> sound_effects;
    std::map<music_t,Mix_Music*> background_songs;
    public:
    AudioPlayer();
    void play(sfx_t sound_effect);
    void play(music_t music);
};
