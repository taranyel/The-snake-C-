#include "Cell.h"

void Cell::setCoordinate(int x, int y) {
    setX(x);
    setY(y);
}

Cell::Cell(int x, int y) : Coordinate(x, y) {}

void Cell::draw(int width, Color color) {
    DrawRectangle(getX(), getY(), width, width, color);
}
