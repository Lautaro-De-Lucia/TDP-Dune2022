#include "./client_texturehandler.h"

TextureHandler::TextureHandler(SDL2pp::Renderer & game_renderer){
    std::string path;
    //  Load Unit Textures
    for(unit_t UNIT: units)
	    for(player_t FACTION: factions)
		    for (direction_t DIR: directions)
                path.append(DATA_PATH)
                .append("/").append("units")
                .append("/").append(stringify(UNIT))
                .append("/").append(stringify(FACTION))
                .append("/").append(stringify(DIR)).append(".png"),
                unit_textures[UNIT][FACTION].emplace(DIR,SDL2pp::Texture(game_renderer,path.c_str())),
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

};

SDL2pp::Texture & TextureHandler::getTexture(unit_t unit, player_t faction, direction_t direction){
    return this->unit_textures[unit][faction].at(direction);
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
