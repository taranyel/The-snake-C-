class Coordinate {
private:
    int x;
    int y;

public:
    Coordinate(int x, int y);

    int getX() const;

    void setX(int newX);

    int getY() const;

    void setY(int newY);
};
