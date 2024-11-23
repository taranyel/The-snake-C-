#include "controller/Game.h"

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600


int main() {
    Screen *screen = new Screen(SCREEN_WIDTH, SCREEN_HEIGHT, "Snake");
    screen->start();
    delete screen;
    return 0;
}

