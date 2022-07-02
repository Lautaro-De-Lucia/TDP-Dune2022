#include "client_shotshandler.h"

ShotsHandler::ShotsHandler(SDL2pp::Renderer& renderer) : renderer(renderer) {

}

ShotsHandler::~ShotsHandler() {

}

void ShotsHandler::updateAttacker(int id, unit_t type, Position attacker_pos, Position target_pos) {

    for (int _id : this->attackers_id) {
        if (_id == id) {
            this->attackers_position[id] = attacker_pos;
            this->attackers_target[id] = target_pos;
            return;
        }
    }

    // not found, adding new:
    this->attackers_id.push_back(id);
    this->attackers_type.insert({id,type});
    this->attackers_position.insert({id,attacker_pos});
    this->attackers_target.insert({id,target_pos});
    this->attackers_shot_cue.insert({id,clock()});

    this->createShot(type, attacker_pos, target_pos);    
}

void ShotsHandler::createShot(unit_t type, Position attacker_pos, Position target_pos) {

    switch (type)
    {
    case TRIKE:
        this->audio.play(SONIC3);
        this->shots.push_back(Shot(attacker_pos, target_pos, TRIKE_BULLET_SPEED, type));
        break;
    case FREMEN:
        this->audio.play(GUN_1);
        this->shots.push_back(Shot(attacker_pos, target_pos, FREMEN_BULLET_SPEED, type));
        break;
    case INFANTRY:
        this->audio.play(GUN_2);
        this->shots.push_back(Shot(attacker_pos, target_pos, INFANTRY_BULLET_SPEED, type));
        break;
    case SARDAUKAR:
        this->audio.play(MISSILE);
        this->shots.push_back(Shot(attacker_pos, target_pos, SARDAUKAR_BULLET_SPEED, type));
        break;
    case TANK:
        this->audio.play(SONIC3);
        this->shots.push_back(Shot(attacker_pos, target_pos, TANK_BULLET_SPEED, type));
        break;
    case DEVASTATOR:
        this->audio.play(SONIC3);
        this->shots.push_back(Shot(attacker_pos, target_pos, DEVASTATOR_BULLET_SPEED, type));
        break;
    
    default:
        break;
    }
}

void ShotsHandler::removeAttacker(int id) {

    bool found = false;
    int index = -1;
    for (size_t i = 0; i < this->attackers_id.size(); i++) {
        if (this->attackers_id[i] == id) {
            found = true;
            index = i;
            break;
        }
    }
    if (found) {
        this->attackers_id.erase(this->attackers_id.begin()+index);
        this->attackers_type.erase(id);
        this->attackers_position.erase(id);
        this->attackers_target.erase(id);
        this->attackers_shot_cue.erase(id);
    }
}


void ShotsHandler::update() {

    bool has_reached_its_target = false;

    std::vector<int> shots_to_remove;

    for (size_t i = 0; i < this->shots.size(); i++) {
        this->shots[i].update(has_reached_its_target);
        if (has_reached_its_target)
            shots_to_remove.push_back(i);
    }

    for (size_t i = 0; i < shots_to_remove.size(); i++) {
        this->attackers_type.erase(i);
        this->attackers_position.erase(i);
        this->attackers_target.erase(i);
        this->attackers_shot_cue.erase(i);
        this->shots.erase(this->shots.begin()+shots_to_remove[i]);
    }

    for (size_t i = 0; i < this->shots.size(); i++)
        this->shots[i].update(has_reached_its_target);

    clock_t current_time = clock();

    for (int id : this->attackers_id) {

        size_t time_since_last_shot = current_time - this->attackers_shot_cue[id];

        switch (this->attackers_type[id])
        {
        case TRIKE:
            if (time_since_last_shot > TRIKE_SHOT_FREQUENCY) {
                this->createShot(this->attackers_type[id], this->attackers_position[id], this->attackers_target[id]);
                this->attackers_shot_cue[id] = clock();
            }
            break;
        case FREMEN:
            if (time_since_last_shot > FREMEN_SHOT_FREQUENCY) {
                this->createShot(this->attackers_type[id], this->attackers_position[id], this->attackers_target[id]);
                this->attackers_shot_cue[id] = clock();
            }
            break;
        case TANK:
            if (time_since_last_shot > TANK_SHOT_FREQUENCY) {
                this->createShot(this->attackers_type[id], this->attackers_position[id], this->attackers_target[id]);
                this->attackers_shot_cue[id] = clock();
            }
            break;
        case DEVASTATOR:
            if (time_since_last_shot > DEVASTATOR_SHOT_FREQUENCY) {
                this->createShot(this->attackers_type[id], this->attackers_position[id], this->attackers_target[id]);
                this->attackers_shot_cue[id] = clock();
            }
            break;
        case INFANTRY:
            if (time_since_last_shot > INFANTRY_SHOT_FREQUENCY) {
                this->createShot(this->attackers_type[id], this->attackers_position[id], this->attackers_target[id]);
                this->attackers_shot_cue[id] = clock();
            }
            break;
        case SARDAUKAR:
            if (time_since_last_shot > SARDAUKAR_SHOT_FREQUENCY) {
                this->createShot(this->attackers_type[id], this->attackers_position[id], this->attackers_target[id]);
                this->attackers_shot_cue[id] = clock();
            }
            break;
        default:
            break;
        }
    }
}

void ShotsHandler::render(SDL2pp::Renderer& renderer, TextureHandler& textures, int cam_pos_x, int cam_pos_y) {

    for (size_t i = 0; i < this->shots.size(); i++) {
        this->shots[i].render(renderer, textures, cam_pos_x, cam_pos_y);
    }
}
