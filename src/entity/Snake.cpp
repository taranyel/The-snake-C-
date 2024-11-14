#include "Snake.h"

Snake::Snake(int startX, int startY, int startSpeedX, int startSpeedY, int snakeWidth) {
    body.push_back(new Coordinate(startX, startY));
    speed = new Coordinate(startSpeedX, startSpeedY);
    length = 1;
    this->width = snakeWidth;
    this->direction = RIGHT;
}

Snake::~Snake() {
    for (Coordinate *coordinate: body) {
        delete coordinate;
    }

    delete speed;
}

std::vector<Coordinate *> Snake::getBody() {
    return body;
}

Coordinate &Snake::getSpeed() const {
    return *speed;
}

int Snake::getLength() const {
    return length;
}

void Snake::setLength(int newLength) {
    Snake::length = newLength;
}

int Snake::getWidth() const {
    return width;
}

Direction Snake::getDirection() const {
    return direction;
}

void Snake::setDirection(Direction newDirection) {
    Snake::direction = newDirection;
}

void Snake::setSpeed(int x, int y) {
    speed->setX(x);
    speed->setY(y);
}

void Snake::grow() {
    int lastX = body.at(length - 1)->getX();
    int lastY = body.at(length - 1)->getY();

    switch (direction) {
        case UP:
            body.push_back(new Coordinate(0, lastY + width));
            break;

        case DOWN:
            body.push_back(new Coordinate(0, lastY - width));
            break;

        case RIGHT:
            body.push_back(new Coordinate(lastX - width, 0));
            break;

        case LEFT:
            body.push_back(new Coordinate(lastX + width, 0));
            break;
    }
    length++;
}




