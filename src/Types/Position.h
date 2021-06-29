#ifndef POSITION_H
#define POSITION_H


class Position
{
public:
    Position();
    Position(bool isValid, int x, int y);

    // Setters
    void setPosition(bool isValid, int x, int y);
    void setInvalid();

    // Getterrs
    bool isInvalid() const;
    int x() const;
    int y() const;

    // Overloaded operators
    bool operator!=(Position other) const;
    bool operator==(Position other) const;

private:
    // Params
    bool _isValid;
    int _x;
    int _y;
};

#endif // POSITION_H
