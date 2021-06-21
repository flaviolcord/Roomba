#include "Battery.h"

Battery::Battery(int level)
{
    _level = level;
}

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
