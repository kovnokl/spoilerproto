#include "Spoilerpos.h"

Spoilerpos::Spoilerpos() {

}

Spoilerpos::Spoilerpos(float posv, float posh)
{
    this->posv = posv;
    this->posh = posh;
}

Spoilerpos Spoilerpos::getDistance(Spoilerpos *pos2)
{
   return Spoilerpos(pos2->posv - this->posv, pos2->posh - this->posh);
}