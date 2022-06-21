#include "client_audioplayer.h"

AudioPlayer::AudioPlayer(){    
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
    {
        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
        exit(1);
    }
    Mix_Chunk * gun1 = Mix_LoadWAV( DATA_PATH GUN1_PATH );
    if( gun1 == NULL )
    {
        printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
        exit(1);
    }
    this->sound_effects.insert({GUN_1,gun1}); 
}

void AudioPlayer::play(sfx_t sound_effect){
    Mix_PlayChannel(-1,this->sound_effects[sound_effect],0);
}

void AudioPlayer::play(music_t sound_effect){
    Mix_PlayMusic(this->sound_effects[sound_effect],-1);
}
