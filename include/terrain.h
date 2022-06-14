#pragma once
#include "common_utils.h"

class Terrain {
 public:
    Terrain();
    virtual ~Terrain(){}
    virtual bool canTraverse(){return false;}
    virtual bool canExtract(){return false;}
    virtual bool canBuild(){return false;}
    virtual size_t extractSpice(){return 0;}
};

class Sand : public Terrain {
    size_t spice = DEF_SAND_SPICE;
 public:
    Sand();
    virtual bool canTraverse();
    virtual bool canExtract();
    virtual bool canBuild();
    virtual size_t extractSpice();
};

class Rock : public Terrain {
 public:
    Rock();
    virtual bool canTraverse();
    virtual bool canExtract();
    virtual bool canBuild();
};

class Pit : public Terrain {
 public:
    Pit();
    virtual bool canTraverse();
    virtual bool canExtract();
    virtual bool canBuild();
};

class Dune : public Terrain {
 public:
    Dune();
    virtual bool canTraverse();
    virtual bool canExtract();
    virtual bool canBuild();
};
