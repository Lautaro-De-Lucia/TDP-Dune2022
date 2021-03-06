#include "client_explosionshandler.h"

ExplosionsHandler::ExplosionsHandler(SDL2pp::Renderer& renderer) : renderer(renderer) {

}

ExplosionsHandler::~ExplosionsHandler() {

}

void ExplosionsHandler::addExplosion(int id, selectable_t type, Position pos) {

    // not found, adding new:
    this->explosions_id.push_back(id);
    this->explosions_type.insert({id,type});
    this->explosions_position.insert({id,pos});

    if ((int) type <= 7) {
        if (type == SEL_INFANTRY)
            this->audio.play(KILLGUY8);
        else if (type == SEL_FREMEN)
            this->audio.play(KILLGUY9);
        else if (type == SEL_SARDAUKAR)
            this->audio.play(KILLGUY7);
        else
            this->audio.play(EXPLSML2);
    } else {
        this->audio.play(EXPLHG1);
    }
    
    this->explosions.push_back(Explosion(type, pos));
}

void ExplosionsHandler::update() {

    bool has_ended = false;

    std::vector<int> explosions_to_remove;

    for (size_t i = 0; i < this->explosions.size(); i++) {
        this->explosions[i].update(has_ended);
        if (has_ended)
            explosions_to_remove.push_back(i);
    }

    if (explosions_to_remove.size() != 0) {
        for (size_t i = explosions_to_remove.size() - 1; i >= 0; i--) {
                this->explosions_type.erase(explosions_to_remove[i]);
                this->explosions_position.erase(explosions_to_remove[i]);
                this->explosions.erase(this->explosions.begin() + explosions_to_remove[i]);
        }
    }

    for (size_t i = 0; i < this->explosions.size(); i++)
        this->explosions[i].update(has_ended);
}

void ExplosionsHandler::render(SDL2pp::Renderer& renderer, TextureHandler& textures, int cam_pos_x, int cam_pos_y) {

    for (size_t i = 0; i < this->explosions.size(); i++) {
        this->explosions[i].render(renderer, textures, cam_pos_x, cam_pos_y);
    }
}
