#include "Environment.h"

// Default values
#define DIMENSION_X 50
#define DIMENSION_y 50

Environment::Environment()
{
    // Default values
    _gradeDimensionX = DIMENSION_X;
    _gradeDimensionY = DIMENSION_y;
}

Environment::Environment(int gradeDimensionX, int gradeDimensionY)
{
    _gradeDimensionX = gradeDimensionX;
    _gradeDimensionY = gradeDimensionY;
}
