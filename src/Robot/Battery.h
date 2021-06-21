#ifndef BATTERY_H
#define BATTERY_H

#include <iostream>
using namespace std;

class Battery
{
public:
    Battery(int level);

    void chargeBattery(int chargeValue);
    void dischargeBattery();
    int getLevel() { return _level;}
private:

    int _level;
};

#endif // BATTERY_H
