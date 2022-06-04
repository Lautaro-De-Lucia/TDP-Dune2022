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

bool Sand::isTraversable(){return occupied;}
bool Sand::isExtractible(){return true;}
bool Sand::canBuild(){return false;}

bool Rock::isTraversable(){return occupied;}
bool Rock::isExtractible(){return false;}
bool Rock::canBuild(){return true;}

bool Pit::isTraversable(){return true;}
bool Pit::isExtractible(){return true;}
bool Pit::canBuild(){return false;}
