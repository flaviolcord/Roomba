#ifndef BATTERY_H
#define BATTERY_H

#include <iostream>
using namespace std;

class Battery
{
public:
    // Class constructors
    Battery(int level);

    // Methods
    void chargeBattery(int chargeValue);
    void dischargeBattery();

    // Getters
    int getLevel() { return _level;}
private:
    // Vars
    int _level;
};

#endif // BATTERY_H
