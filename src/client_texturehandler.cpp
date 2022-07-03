#include "./client_texturehandler.h"

TextureHandler::TextureHandler(SDL2pp::Renderer & game_renderer):
creator_mark(game_renderer,DATA_PATH CMARK_PATH)
{
    std::string path;
    //  Load Unit Textures

    for(unit_t UNIT: units)
	    for(player_t FACTION: factions)
		    for (direction_t DIR: directions)
                for (frame_t ANIMATION : animations)
                    path.append(DATA_PATH)
                    .append("/").append("units")
                    .append("/").append(stringify(UNIT))
                    .append("/").append(stringify(FACTION))
                    .append("/").append(stringify(ANIMATION))
                    .append("/").append(stringify(DIR)).append(".png"),
                    std::cout << "Loading path" << path << std::endl,
                    unit_textures[UNIT][FACTION][ANIMATION].emplace(DIR,SDL2pp::Texture(game_renderer,path.c_str())),
                    path.clear();

    //  Load Building Textures
    for(building_t BUILDING: buildings)
	    for(player_t FACTION: factions)
            path
                .append(DATA_PATH)
                .append("/").append("buildings")
                .append("/").append(stringify(BUILDING))
                .append("/").append(stringify(FACTION))
                .append("/").append(".png"),
                building_textures[BUILDING].emplace(FACTION,SDL2pp::Texture(game_renderer,path.c_str())),
                path.clear();          
    //  Load Button Info Textures
    for(unit_t UNIT: units)
        path.append(DATA_PATH)
            .append("/").append("buttoninfo")
            .append("/").append(stringify(UNIT)).append(".png"),
            unit_info_textures.emplace(UNIT,SDL2pp::Texture(game_renderer,path.c_str())),
            path.clear();
    for(building_t BUILDING: buildings)
        path.append(DATA_PATH)
            .append("/").append("buttoninfo")
            .append("/").append(stringify(BUILDING)).append(".png"),
            building_info_textures.emplace(BUILDING,SDL2pp::Texture(game_renderer,path.c_str())),
            path.clear();
    //  Load Textures for map cells
        for(std::string tile : maptiles)
            path.append(DATA_PATH).append("/").append("mapsprites").append("/").append(tile.c_str()).append(".png"),
            cell_textures.emplace(tile,SDL2pp::Texture(game_renderer,path.c_str())),
            path.clear();
    //  Load Textures for the progress bar
        for(size_t i = 0 ; i < 10 ; i++){
            path.append(DATA_PATH).append("/").append("extrasprites").append("/").append("creation").append("/").append(std::to_string(i)).append(".png");
            progress_bar_textures.push_back(SDL2pp::Texture(game_renderer,path.c_str()));
            path.clear();
        }
    //  Load Textures for unit images
    for(unit_t UNIT: units)
	    for(player_t FACTION: factions)
                path.append(DATA_PATH)
                .append("/").append("extrasprites")
                .append("/").append("unit_imgs")
                .append("/").append(stringify(FACTION))
                .append("/").append(stringify(UNIT)).append(".jpg"),
                std::cout << "Loading texture at path: " << path << std::endl,
                unit_imgs_textures[FACTION].emplace(UNIT,SDL2pp::Texture(game_renderer,path.c_str())),
                path.clear();
    //  Load Textures for explosions
    for(size_t i = 0 ; i < 20 ; i++) {
        path.append(DATA_PATH)
        .append("/").append("explosions")
        .append("/").append("explosion")
        .append(std::to_string(i)).append(".png"),
        this->explosion_textures.push_back(SDL2pp::Texture(game_renderer,path.c_str())),
        path.clear();
    }
    for(size_t i = 0 ; i < 20 ; i++) {
        path.append(DATA_PATH)
        .append("/").append("explosions")
        .append("/").append("blood")
        .append(std::to_string(i)).append(".png"),
        this->blood_textures.push_back(SDL2pp::Texture(game_renderer,path.c_str())),
        path.clear();
    }
    //  Load Textures for shots
    for(unit_t UNIT: units) {
        if (UNIT == HARVESTER)
            continue;
        path.append(DATA_PATH)
        .append("/").append("unitshots")
        .append("/").append(stringify(UNIT)).append("/shot.png"),
        this->unit_shot_textures.emplace(UNIT,SDL2pp::Texture(game_renderer,path.c_str())),
        path.clear();
    }

};

SDL2pp::Texture & TextureHandler::getTexture(unit_t unit, player_t faction,frame_t animation, direction_t direction){
    return this->unit_textures[unit][faction][animation].at(direction);
}

SDL2pp::Texture & TextureHandler::getTexture(building_t building, player_t faction){
    return this->building_textures[building].at(faction);
}
SDL2pp::Texture & TextureHandler::getInfo(unit_t unit){
    return this->unit_info_textures.at(unit);
}
SDL2pp::Texture & TextureHandler::getInfo(building_t building){
    return this->building_info_textures.at(building);
}

SDL2pp::Texture & TextureHandler::getCell(std::string & type){
    return this->cell_textures.at(type);
}

SDL2pp::Texture & TextureHandler::getCreationProgress(int percentage){
    if(percentage > 110)
        return this->progress_bar_textures[9]; 
    for(size_t i = 0 ; i < 10 ; i++)
        if(12 * i < percentage && percentage <= 12*(i+1))
            return this->progress_bar_textures[i];
    throw std::runtime_error("Not a valid percentage number");
}

SDL2pp::Texture & TextureHandler::getUnitIMG(player_t faction, unit_t unit){
    return this->unit_imgs_textures[faction].at(unit);
}

SDL2pp::Texture & TextureHandler::getCreatorMark() {
    return this->creator_mark;
}

SDL2pp::Texture & TextureHandler::getExplosion(int frame, bool is_infantry){
    if (is_infantry)
        return this->blood_textures[frame];
    else
        return this->explosion_textures[frame];
}

SDL2pp::Texture & TextureHandler::getUnitShot(unit_t unit){
    return this->unit_shot_textures.at(unit);
}
