#ifndef LASER_H
#define LASER_H

#include "Environment/Environment.h"

class Laser
{
public:
     // Class constructors
    Laser(Environment *environment);

    // Methods
    bool checkObstacle(Position pos);

private:
    // Vars
    Environment *_environment;
};

#endif // LASER_H
