#include "raylib.h"

class Window {
private:
    int width;
    int height;
    const char* title;

public:
    Window(int width, int height, const char *title);

    void initWindow();

    ~Window();

    void closeWindow();

    int getWidth() const;

    int getHeight() const;

    void setWidth(int newWidth);

    void setHeight(int newHeight);
};