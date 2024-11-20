#include "raylib.h"

class Board {
private:
    int width;
    int height;
    const char* title;
    int cellSize;

public:
    Board(int width, int height, const char *title, int cellSize);

    int getCellSize() const;

    void initWindow();

    ~Board();

    void closeWindow();

    int getWidth() const;

    int getHeight() const;

    void setWidth(int newWidth);

    void setHeight(int newHeight);
};