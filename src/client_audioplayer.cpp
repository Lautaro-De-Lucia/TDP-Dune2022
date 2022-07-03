#include "client_audioplayer.h"

#include <iostream>

AudioPlayer::AudioPlayer(){
   
    if (Mix_OpenAudio((int) 44100, (Uint16) MIX_DEFAULT_FORMAT, (int) 2, (int) 2048) < 0) {
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

    Mix_Chunk* mortar1 = Mix_LoadWAV(DATA_PATH MORTAR1_PATH);
    if (mortar1 == NULL) {
        printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
        exit(1);
    }
    this->sound_effects.insert({MORTAR1,mortar1});

    Mix_Chunk* medtank1 = Mix_LoadWAV(DATA_PATH MEDTANK1_PATH);
    if (medtank1 == NULL) {
        printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
        exit(1);
    }
    this->sound_effects.insert({MEDTANK1,medtank1});

    Mix_Chunk* crush1 = Mix_LoadWAV(DATA_PATH CRUSH1_PATH);
    if (crush1 == NULL) {
        printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
        exit(1);
    }
    this->sound_effects.insert({CRUSH1,crush1});

    Mix_Chunk* killguy7 = Mix_LoadWAV(DATA_PATH KILLGUY7_PATH);
    if (killguy7 == NULL) {
        printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
        exit(1);
    }
    this->sound_effects.insert({KILLGUY7,killguy7});

    Mix_Chunk* killguy8 = Mix_LoadWAV(DATA_PATH KILLGUY8_PATH);
    if (killguy8 == NULL) {
        printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
        exit(1);
    }
    this->sound_effects.insert({KILLGUY8,killguy8});

    Mix_Chunk* killguy9 = Mix_LoadWAV(DATA_PATH KILLGUY9_PATH);
    if (killguy9 == NULL) {
        printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
        exit(1);
    }
    this->sound_effects.insert({KILLGUY9,killguy9});



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
