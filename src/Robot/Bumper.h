#ifndef BUMPER_H
#define BUMPER_H

#include "src/Environment/Environment.h"

class Bumper
{

public:
    Bumper(Environment *environment);

    // Methodes
    bool checkObstacle();
private:

    Environment *_environment;
};

#endif // BUMPER_H
