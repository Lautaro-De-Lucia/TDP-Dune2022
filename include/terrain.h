#pragma once

class Terrain {
 public:
    bool occupied;
    Terrain();
    bool isOccupied();
    void occupy();
    void free();
    bool isTraversable();
    bool isExtractible();
    bool canBuild();
};

class Sand : public Terrain {
 public:
    bool isTraversable();
    bool isExtractible();
    bool canBuild();
};

class Rock : public Terrain {
 public:
    bool isTraversable();
    bool isExtractible();
    bool canBuild();
};

class Pit : public Terrain {
 public:
    bool isTraversable();
    bool isExtractible();
    bool canBuild();
};
