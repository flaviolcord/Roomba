#include "Environment.h"
#include <fstream>


// Default values
#define DIMENSION_X 25
#define DIMENSION_y 25

Environment::Environment()
{
    // Default values
    _gridDimensionX = DIMENSION_X;
    _gridDimensionY = DIMENSION_y;

    allocateGridMemory();
}

Environment::Environment(int gridDimensionX, int gridDimensionY)
{
    _gridDimensionX = gridDimensionX;
    _gridDimensionY = gridDimensionY;

    allocateGridMemory();
}

void Environment::allocateGridMemory()
{
    _grid = new int* [_gridDimensionY];

    for(int i = 0; i < _gridDimensionY; i++)
    {
        _grid[i] = new int [_gridDimensionX];
    }

    // Fill grid with zeros to start
    for(int i = 0; i < _gridDimensionY; i++) {
        for(int j = 0; j < _gridDimensionX; j++) {
            _grid[i][j] = 0;
        }
    }
}

Environment::Environment(string fileName)
{
    setGridFromFile(fileName);
}

void Environment::setGridFromFile(string fileName)
{
    string line;
    ifstream environmentFile;

    environmentFile.open (fileName);

    if(environmentFile.is_open())
    {
        while(getline(environmentFile, line))
        {
            _fileLines.push_back(line);
        }
        environmentFile.close();
    } else
    {
        cout<<"ERROR! << Environment file >>"<<endl;
    }

    // Allocate grid memory
    _gridDimensionY = _fileLines.front().size();
    _gridDimensionX = _fileLines.size();

    allocateGridMemory();

    // Fill grid
    string elementValue;
    list<string>::iterator it = _fileLines.begin();
    for(int i = 0; i < _fileLines.size(); i++) {
        for(int j = 0; j < (*it).size(); j++)
        {
            elementValue = (*it).at(j);
            _grid[i][j] = std::stoi(elementValue, nullptr, 10);
        }
        it++;
    }
}

void Environment::printGrid() {

    for(int i = 0; i < _gridDimensionY; i++) {
        for(int j = 0; j < _gridDimensionX; j++) {
            cout<<"  "<<_grid[i][j];
        }
        cout<<endl;
    }
}
