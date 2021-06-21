#include "Bumper.h"

#define OBSTACLE 1

Bumper::Bumper(Environment *environment)
{
    _environment = environment;
}

bool Bumper::checkObstacle(Position pos)
{
    if(_environment->getPosValue(pos) == OBSTACLE) { return true;}

    return false;
}
