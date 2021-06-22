#ifndef BUMPER_H
#define BUMPER_H

#include "Environment/Environment.h"

class Bumper
{

public:
    // Class constructors
    Bumper(Environment *environment);

    // Methods
    bool checkObstacle(Position pos);
private:
    // Vars
    Environment *_environment;
};

#endif // BUMPER_H
