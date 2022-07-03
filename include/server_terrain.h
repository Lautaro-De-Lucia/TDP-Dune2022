#pragma once

#include "common_utils.h"

class Terrain {
 public:
    Terrain();
    virtual ~Terrain(){}
    virtual bool canTraverse(){return false;}
    virtual bool canExtract(){return false;}
    virtual bool canBuild(){return false;}
    virtual bool canSlowDown(){return false;}
    virtual size_t extractSpice(){return 0;}
    virtual int getSpice(){return 0;}
};

class Sand : public Terrain {
    size_t spice;
 public:
    Sand(int init_spice);
    virtual bool canTraverse();
    virtual bool canExtract();
    virtual bool canBuild();
    virtual bool canSlowDown();
    virtual size_t extractSpice();
    virtual int getSpice();
};

class Rock : public Terrain {
 public:
    Rock();
    virtual bool canTraverse();
    virtual bool canExtract();
    virtual bool canBuild();
    virtual bool canSlowDown();
    virtual int getSpice();
};

class Pit : public Terrain {
 public:
    Pit();
    virtual bool canTraverse();
    virtual bool canExtract();
    virtual bool canBuild();
    virtual bool canSlowDown();
    virtual int getSpice();
};

class Dune : public Terrain {
 public:
    Dune();
    virtual bool canTraverse();
    virtual bool canExtract();
    virtual bool canBuild();
    virtual bool canSlowDown();
    virtual int getSpice();
};
