#include "Snake.h"

Snake::Snake(int startX, int startY, int speed, int snakeWidth) {
    body.push_back(new Cell(startX, startY));
    this->speed = speed;
    length = 1;
    width = snakeWidth;
    direction = RIGHT;
}

Snake::~Snake() {
    for (Cell *bodyPart: body) {
        delete bodyPart;
    }
}

std::vector<Cell *> Snake::getBody() {
    return body;
}

int Snake::getSpeed() const {
    return speed;
}

int Snake::getLength() const {
    return length;
}

int Snake::getWidth() const {
    return width;
}
void Snake::setDirection(Direction newDirection) {
    direction = newDirection;
}

void Snake::setSpeed(int newSpeed) {
    speed = newSpeed;
}

void Snake::grow() {
    int lastX = body.at(length - 1)->getX();
    int lastY = body.at(length - 1)->getY();

    switch (direction) {
        case UP:
            body.push_back(new Cell(lastX, lastY + width));
            break;

        case DOWN:
            body.push_back(new Cell(lastX, lastY - width));
            break;

        case RIGHT:
            body.push_back(new Cell(lastX - width, lastY));
            break;

        case LEFT:
            body.push_back(new Cell(lastX + width, lastY));
            break;
    }
    length++;
}

Direction Snake::getDirection() const {
    return direction;
}
