#include "Position.h"

Position::Position(){
    setInvalid();
}

Position::Position(bool isValid, int x, int y)
{
    _isValid = isValid;
    _x = x;
    _y = y;
}

void Position::setPosition(bool isValid, int x, int y)
{
    _isValid = isValid;
    _x = x;
    _y = y;
}

void Position::setInvalid()
{
    _isValid = false;
    _x = 0;
    _y = 0;
}

bool Position::isInvalid() const{
    return !_isValid;
}

int Position::x() const {
    return(_x);
}

int Position::y() const {
    return(_y);
}

bool Position::operator!=(Position other) const {
    return (other.isInvalid() != isInvalid() || x() != other.x() || y() != other.y());
}

bool Position::operator==(Position other) const {
    return (other.isInvalid() == isInvalid() && x() == other.x() && y() == other.y());
}
