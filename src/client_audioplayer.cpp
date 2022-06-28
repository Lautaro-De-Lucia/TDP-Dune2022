#include "client_audioplayer.h"

#include <iostream>

AudioPlayer::AudioPlayer(){
   
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
        exit(1);
    }

    // SOUND EFFECTS

    Mix_Chunk* house_chosen = Mix_LoadWAV(DATA_PATH HOUSE_CHOSEN_PATH);
    if (house_chosen == NULL) {
        printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
        exit(1);
    }
    this->sound_effects.insert({HOUSE_CHOSEN,house_chosen});

    Mix_Chunk* game_start = Mix_LoadWAV(DATA_PATH GAME_START_PATH);
    if (game_start == NULL) {
        printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
        exit(1);
    }
    this->sound_effects.insert({GAME_START,game_start});

    Mix_Chunk* gun1 = Mix_LoadWAV(DATA_PATH GUN1_PATH);
    if (gun1 == NULL) {
        printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
        exit(1);
    }
    this->sound_effects.insert({GUN_1,gun1});

    Mix_Chunk* gun2 = Mix_LoadWAV(DATA_PATH GUN2_PATH);
    if (gun2 == NULL) {
        printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
        exit(1);
    }
    this->sound_effects.insert({GUN_2,gun2});


    Mix_Chunk* missile = Mix_LoadWAV(DATA_PATH MISSILE_PATH);
    if (missile == NULL) {
        printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
        exit(1);
    }
    this->sound_effects.insert({MISSILE,missile});

    Mix_Chunk* explhg = Mix_LoadWAV(DATA_PATH EXPLHG1_PATH);
    if (explhg == NULL) {
        printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
        exit(1);
    }
    this->sound_effects.insert({EXPLHG1,explhg});

    Mix_Chunk* rocket1 = Mix_LoadWAV(DATA_PATH ROCKET1_PATH);
    if (rocket1 == NULL) {
        printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
        exit(1);
    }
    this->sound_effects.insert({ROCKET1,rocket1});

    Mix_Chunk* explsml2 = Mix_LoadWAV(DATA_PATH EXPLSML2_PATH);
    if (explsml2 == NULL) {
        printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
        exit(1);
    }
    this->sound_effects.insert({EXPLSML2,explsml2});

    Mix_Chunk* sonic3 = Mix_LoadWAV(DATA_PATH SONIC3_PATH);
    if (sonic3 == NULL) {
        printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
        exit(1);
    }
    this->sound_effects.insert({SONIC3,sonic3});

    // MUSIC


    Mix_Music* atreides_music = Mix_LoadMUS(DATA_PATH ATREIDES_MUSIC_PATH);
    if (atreides_music == NULL) {
        printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
        exit(1);
    }
    this->background_songs.insert({ATREIDES_MUSIC, atreides_music});

    Mix_Music* harkonnen_music = Mix_LoadMUS(DATA_PATH HARKONNEN_MUSIC_PATH);
    if (harkonnen_music == NULL) {
        printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
        exit(1);
    }
    this->background_songs.insert({HARKONNEN_MUSIC, harkonnen_music});

    Mix_Music* ordos_music = Mix_LoadMUS(DATA_PATH ORDOS_MUSIC_PATH);
    if (ordos_music == NULL) {
        printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
        exit(1);
    }
    this->background_songs.insert({ORDOS_MUSIC, ordos_music});

    Mix_Music* waiting_music = Mix_LoadMUS(DATA_PATH WAITING_MUSIC_PATH);
    if (waiting_music == NULL) {
        printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
        exit(1);
    }
    this->background_songs.insert({WAITING_MUSIC, waiting_music});
}

void AudioPlayer::play(sfx_t sound_effect){
    Mix_PlayChannel(-1,this->sound_effects[sound_effect],0);
}

void AudioPlayer::play(music_t music){
    Mix_PlayMusic(this->background_songs[music],-1);
}
