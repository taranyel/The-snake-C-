#include "raylib.h"
#include "ScreenType.h"
#include "../controller/Game.h"

class Screen {
private:
    int width;
    int height;
    const char* title;
    ScreenType screenType;
    Game *game;

public:
    Screen(int width, int height, const char *title);

    void initWindow();

    ~Screen();

    void showStartScreen();

    void showGameOverScreen();

    void showVictoryScreen();

    void start();
};