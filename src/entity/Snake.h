#pragma once
#include "Cell.h"

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

    int getSpeed() const;

    void setSpeed(int newSpeed);

    int getLength() const;

    Direction getDirection() const;

    int getWidth() const;

    void setDirection(Direction newDirection);

    void grow();
};

