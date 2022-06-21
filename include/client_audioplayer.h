#include <SDL2/SDL_mixer.h>
#include <map>

#define GUN1_PATH "/sounds/20MMGUN1.WAV"

enum sfx_t {
    GUN_1,
    EXPLOSION_1
};

enum music_t {
    BACKGROUND_MUSIC
};

class AudioPlayer {
    private:
    std::map<sfx_t,Mix_Chunk*> sound_effects;
    std::map<music_t,Mix_Music*> music;
    public:
    AudioPlayer();
    void play(sfx_t sound_effect);
    void play(music_t sound_effect);
};