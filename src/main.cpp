#include "screen/Screen.h"

#define SCREEN_WIDTH 660
#define SCREEN_HEIGHT 690


int main() {
    Screen *screen = new Screen(SCREEN_WIDTH, SCREEN_HEIGHT, "Snake");
    screen->start();
    delete screen;
    return 0;
}