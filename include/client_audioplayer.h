#pragma once

#include <SDL2/SDL_mixer.h>
#include <map>

#define ATREIDES_MUSIC_PATH "/atreides-background.wav"
#define HARKONNEN_MUSIC_PATH "/harkonnen-background.wav"
#define ORDOS_MUSIC_PATH "/ordos-background.wav"
#define WAITING_MUSIC_PATH "/waiting.wav"

#define HOUSE_CHOSEN_PATH "/sounds/house-chosen.wav"
#define GAME_START_PATH "/sounds/game-start.wav"
#define GUN1_PATH "/sounds/20MMGUN1.WAV"
#define GUN2_PATH "/sounds/MGUN2.WAV"
#define MISSILE_PATH "/sounds/MISSLE1.WAV"
#define EXPLHG1_PATH "/sounds/EXPLHG1.WAV"
#define ROCKET1_PATH "/sounds/ROCKET1.WAV"
#define EXPLSML2_PATH "/sounds/EXPLSML2.WAV"
#define SONIC3_PATH "/sounds/SONIC3.WAV"

#define MORTAR1_PATH "/sounds/MORTAR1.WAV"
#define MEDTANK1_PATH "/sounds/MEDTANK1.WAV"

#define CRUSH1_PATH "/sounds/CRUSH1.WAV"

#define KILLGUY7_PATH "/sounds/KILLGUY7.WAV"
#define KILLGUY8_PATH "/sounds/KILLGUY8.WAV"
#define KILLGUY9_PATH "/sounds/KILLGUY9.WAV"

enum sfx_t {
    HOUSE_CHOSEN,
    GAME_START,
    GUN_1,
    GUN_2,
    MISSILE,
    EXPLHG1,
    ROCKET1,
    EXPLSML2,
    SONIC3,
    MORTAR1,
    MEDTANK1,
    CRUSH1,
    KILLGUY7,
    KILLGUY8,
    KILLGUY9
};

enum music_t {
    ATREIDES_MUSIC,
    HARKONNEN_MUSIC,
    ORDOS_MUSIC,
    WAITING_MUSIC
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
