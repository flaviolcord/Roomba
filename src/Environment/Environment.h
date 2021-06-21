#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <iostream>
#include <string.h>
#include <list>
#include "src/Types/Position.h"

using namespace std;

class Environment
{

public:
    // Class constructors
    Environment();
    Environment(int gridDimensionX, int gridDimensionY);
    Environment(string fileName);

    // if rectangle = false (obstacle is a cell) / retangle = true -> obstacle is a rectangle
    void addObstacle(bool rectangle);
    void addObstacle(string fileName);

    // getters
    Position getStationPos() { return _stationPos;}
    Position getRobotPos() {return _robotPosition;}
    int getPosValue(Position pos) {return _grid[pos.x()][pos.y()];}

    // setters
    void setGridValue(Position pos, int value) {_grid[pos.x()][pos.y()] = value;}

    void printGrid();

private:
    // Var
    Position _stationPos;
    Position _robotPosition;
    int _gridDimensionX;
    int _gridDimensionY;
    int **_grid;

    list<string> _fileLines;

    // Methodes
    void allocateGridMemory();
    void setGridFromFile(string fileName);

};

#endif // ENVIRONMENT_H
