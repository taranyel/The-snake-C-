#include <vector>
#include "Direction.h"

class Snake {
private:
    std::vector<Coordinate *> body;
    Coordinate *speed;
    enum Direction direction;
    int length;
    int width;

public:
    Snake(int startX, int startY, int startSpeedX, int startSpeedY, int snakeWidth);

    virtual ~Snake();

    std::vector<Coordinate *> getBody();

    Coordinate &getSpeed() const;

    void setSpeed(int x, int y);

    int getLength() const;

    void setLength(int newLength);

    int getWidth() const;

    Direction getDirection() const;

    void setDirection(Direction newDirection);

    void grow();
};

