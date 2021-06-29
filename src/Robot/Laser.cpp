#include "Laser.h"

#define OBSTACLE 1

Laser::Laser(Environment *environment)
{
    _environment = environment;
}

bool Laser::checkObstacle(Position pos)
{
    if(_environment->getPosValue(pos) == OBSTACLE) { return true;}

    return false;
}
