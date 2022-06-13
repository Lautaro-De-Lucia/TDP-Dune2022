#include "terrain.h"

Terrain::Terrain(){
    this->occupied = false;
}

bool Terrain::isOccupied(){
    return this->occupied;
}
void Terrain::occupy(){
    this->occupied = true;
}
void Terrain::free(){
    this->occupied = false;
}

Sand::Sand():Terrain(){} 
bool Sand::canTraverse(){return true;}
bool Sand::canExtract(){return this->spice > 0 ? true : false;}
bool Sand::canBuild(){return false;}
size_t Sand::extractSpice(){
    if(this->spice > 0){
        std::cout << "Remaining spice:" << this->spice << std::endl;
        this->spice--;
        return 1;
    }
    return 0;
}


Rock::Rock():Terrain(){} 
bool Rock::canTraverse(){return !occupied;}
bool Rock::canExtract(){return false;}
bool Rock::canBuild(){return true;}

Dune::Dune():Terrain(){} 
bool Dune::canTraverse(){return true;}
bool Dune::canExtract(){return false;}
bool Dune::canBuild(){return false;}

Pit::Pit():Terrain(){}
bool Pit::canTraverse(){return false;}
bool Pit::canExtract(){return false;}
bool Pit::canBuild(){return false;}
