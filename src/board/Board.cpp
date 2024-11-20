#include "Board.h"

Board::Board(int width, int height, const char *title, int cellSize) {
    this->width = width;
    this->height = height;
    this->title = title;
    this->cellSize = cellSize;
}

void Board::initWindow() {
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(width, height, title);
    SetTargetFPS(60);
}

Board::~Board() = default;

void Board::closeWindow() {

}

int Board::getWidth() const {
    return width;
}

int Board::getHeight() const {
    return height;
}

void Board::setWidth(int newWidth) {
    Board::width = newWidth;
}

void Board::setHeight(int newHeight) {
    Board::height = newHeight;
}

int Board::getCellSize() const {
    return cellSize;
}




