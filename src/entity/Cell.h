#include "Direction.h"
#include "../board/Board.h"

class Cell : public Coordinate{
public:
    Cell(int x, int y);

    void setCoordinate(int x, int y);

    void draw(int width, Color color);
};
