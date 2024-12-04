#pragma once

#include "Cell.h"
#include "../controller/Direction.h"

class Snake {
private:
    std::vector<Cell *> body;
    int speed;
    Direction direction;
    int length;
    int width;

public:
    Snake(int startX, int startY, int speed, int snakeWidth);

    virtual ~Snake();

    std::vector<Cell *> getBody();

    Cell* getHead();

    int getSpeed() const;

    void setSpeed(int newSpeed);

    int getLength() const;

    Direction getDirection() const;

    void setDirection(Direction newDirection);

    void grow();
};

