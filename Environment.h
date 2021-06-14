#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <iostream>
#include <string.h>
#include <list>

using namespace std;

class Environment
{

public:
    Environment();
    Environment(int gridDimensionX, int gridDimensionY);
    Environment(string fileName);

    // if rectangle = false (obstacle is a cell) / retangle = true -> obstacle is a rectangle
    void addObstacle(bool rectangle);
    void addObstacle(string fileName);
    void printGrid();

private:
    // Var
    int _gridDimensionX;
    int _gridDimensionY;
    int **_grid;

    list<string> _fileLines;

    // Methodes
    void allocateGridMemory();
    void setGridFromFile(string fileName);

};

#endif // ENVIRONMENT_H
