#include "Window.h"

Window::Window(int width, int height, const char *title) {
    this->width = width;
    this->height = height;
    this->title = title;
}

void Window::initWindow() {
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(width, height, title);
    SetTargetFPS(60);
}

Window::~Window() {
    delete title;
}

void Window::closeWindow() {

}

int Window::getWidth() const {
    return width;
}

int Window::getHeight() const {
    return height;
}

void Window::setWidth(int newWidth) {
    Window::width = newWidth;
}

void Window::setHeight(int newHeight) {
    Window::height = newHeight;
}




