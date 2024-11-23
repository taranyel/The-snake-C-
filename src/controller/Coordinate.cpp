#include "Coordinate.h"

Coordinate::Coordinate(int x, int y) : x(x), y(y) {}

int Coordinate::getX() const {
    return x;
}

void Coordinate::setX(int newX) {
    Coordinate::x = newX;
}

int Coordinate::getY() const {
    return y;
}

void Coordinate::setY(int newY) {
    Coordinate::y = newY;
}
