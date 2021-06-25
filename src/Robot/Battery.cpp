#include "Battery.h"

Battery::Battery(int level)
{
    _level = level;
}

// For each action the battery discharge one unit of charge
void Battery::dischargeBattery()
{
    if(_level >= 1)
    {
        _level -= 1;
    } else
    {
        std::cout<<"Battery discharged!"<<endl;
    }
}

void Battery::chargeBattery(int chargeValue)
{
    _level += chargeValue;
}
