#include "server_terrain.h"

Terrain::Terrain(){}

Sand::Sand(int init_spice):Terrain(){
    this->spice = init_spice;
} 
bool Sand::canTraverse(){return true;}
bool Sand::canExtract(){return this->spice > 0 ? true : false;}
bool Sand::canBuild(){return false;}
size_t Sand::extractSpice(){
    if(this->spice > 0){
        this->spice--;
        return 1;
    }
    return 0;
}
int Sand::getSpice(){return this->spice;}



Rock::Rock():Terrain(){} 
bool Rock::canTraverse(){return true;}
bool Rock::canExtract(){return false;}
bool Rock::canBuild(){return true;}
int Rock::getSpice(){return 0;}

Dune::Dune():Terrain(){} 
bool Dune::canTraverse(){return true;}
bool Dune::canExtract(){return false;}
bool Dune::canBuild(){return false;}
int Dune::getSpice(){return 0;}

Pit::Pit():Terrain(){}
bool Pit::canTraverse(){return false;}
bool Pit::canExtract(){return false;}
bool Pit::canBuild(){return false;}
int Pit::getSpice(){return 0;}
