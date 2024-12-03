#include "Cell.h"

Cell::Cell(int x, int y) : Coordinate(x, y) {}

void Cell::setCoordinate(int x, int y) {
    setX(x);
    setY(y);
}

void Cell::draw(int width, Color color) {
    DrawRectangle(getX(), getY(), width, width, color);
}