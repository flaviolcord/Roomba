#include "Environment.h"
#include <fstream>
#include <chrono>
#include <random>


// Default values
#define DIMENSION_X 25
#define DIMENSION_y 25
#define STATION_POS_X 1
#define STATION_POS_Y 0
#define UNCLEAN 0

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

Environment::Environment(string fileName)
{
    setGridFromFile(fileName);
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

    _stationPos = Position(true, STATION_POS_X, STATION_POS_Y);
    _grid[_stationPos.y()][_stationPos.x()] = 2; // Define station in the DEFAULT cell
}

void Environment::setGridFromFile(string fileName)
{
    string line;
    ifstream environmentFile;

    environmentFile.open (fileName);

    // Check file is open
    if(environmentFile.is_open())
    {
        while(getline(environmentFile, line))
        {
            // puch each line to the list
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
            if(_grid[i][j] != 2) {
                _grid[i][j] = std::stoi(elementValue, nullptr, 10);
            }
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

int Environment::generate_random(int minimum, int maximum)
{
    //Generation of random number between a range given by 'minimum' and 'maximum'

    //Generate seed
    std::random_device rd;
    std::default_random_engine eng;
    unsigned int t = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::seed_seq seed{ rd(), t };
    eng.seed(seed);
    std::mt19937 rng(eng());

    // Get distribution
    uniform_int_distribution <int> distribution(minimum, maximum);
    int variable_sorted = distribution(rng);
    distribution.reset();
    return variable_sorted;
}

void Environment::addObstacle(bool rectangle) //add rectangle or cell as an obstacle
{

    if (rectangle)
    {
        // Generate random values for the rectangle range, with the minimum of 3 cells of length
        int rectangle_init_x = generate_random(0, _gridDimensionX-3);
        int rectangle_init_y = generate_random(0, _gridDimensionY-3);
        int rectangle_final_x = generate_random(rectangle_init_x+2, _gridDimensionX-1);
        int rectangle_final_y = generate_random(rectangle_init_y+2, _gridDimensionY-1);

        // if the generated values for the rectangle coincides with the station point, then change the rectangle position (or size)
        while(((rectangle_init_x<=_stationPos.x()) && (_stationPos.x()<=rectangle_final_x)) && ((rectangle_init_y<=_stationPos.y()) && (_stationPos.y()<=rectangle_final_y)))
        {
            //check if the initial x value is too close to the boundary of the grid, if so, than reduce the rectangle "x" coordinates
            if ((_gridDimensionX-rectangle_init_x)==2)
            {
                rectangle_init_x -= 1;
                rectangle_final_x -= 1;
            }
            //if not, than increase the rectangle "x" initial coordinate
            else
            {
                rectangle_init_x += 1;
            }
            // if the rectangle length is less than 2 blocks, increase the final position to make with at least 3 blocks.
            if ((rectangle_final_x - rectangle_init_x)<2)
            {
                rectangle_final_x += 1;
            }
        }
        // for the final rectangle range values, set to 1 the obstacles

        for (int i=rectangle_init_y; i<=rectangle_final_y; i++)
        {
            for (int j=rectangle_init_x; j<=rectangle_final_x; j++)
            {
                _grid[i][j] = 1;
            }
        }
    }


    else
    {
        //create cell in a random position and check if it matches the station position.
        //While it does, the code will generate other cell position in a random manner.
        int cell_x = rand() %_gridDimensionX + 0; //initialize cell x position in the grid
        int cell_y = rand() %_gridDimensionY + 0; //initialize cell y position in the grid
        while(cell_x!=_stationPos.x() && cell_y!=_stationPos.y())
        {
            cell_x = rand() %_gridDimensionX + 0;
            cell_y = rand() %_gridDimensionY + 0;
        }
        _grid[cell_y][cell_x] = 1; //set 1 to the cell obstacle
    }
}

void Environment::addObstacle(string fileName) //read obstacle archive with positions
{

    string line;
    ifstream obstacleFile;

    obstacleFile.open (fileName);

    if(obstacleFile.is_open())
    {
        while(getline(obstacleFile, line))
        {
            _fileLines.push_back(line);
        }
        obstacleFile.close();
    } else
    {
        cout<<"ERROR! << Obstacle file >>"<<endl;
    }

    // Fill grid obstacles
    string elementValue0;
    string elementValue1;
    list<string>::iterator it = _fileLines.begin();
    for(int i = 0; i < _fileLines.size(); i++)
    {
        elementValue0 = (*it).at(0);
        elementValue1 = (*it).at(2);
        _grid[stoi(elementValue0)][stoi(elementValue1)] = 1; //fill obstacles positions in grid with 1
        it++;
    }
}

int Environment::get_clean_cells()
{
    // compute number of cells to be cleaned
    int _cellUnclean(0);
    for (int i=0;i<_gridDimensionY;i++)
    {
        for (int j=0;j<_gridDimensionX;j++)
        {
            if (_grid[i][j] == UNCLEAN)
            {
                _cellUnclean++;
            }
        }
    }
    return _cellUnclean;
}
