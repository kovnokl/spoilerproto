#ifndef SPOILERPOS_H
#define SPOILERPOS_H

#include <Arduino.h>

class Spoilerpos
{
    public:
        float posv = 0;
        float posh = 0;
        Spoilerpos();
        Spoilerpos(float posv, float posh);
        Spoilerpos getDistance(Spoilerpos *pos2);
};

#endif