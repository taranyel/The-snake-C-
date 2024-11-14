#include "../entity/Snake.h"
#include "../window/Window.h"

class Game {
private:
    Snake* snake;
    Window* window;

public:
    Game(int screenWidth, int screenHeight, const char* title, int startSpeed, int snakeWidth);

    ~Game();

    void startGame();

    void endGame();

    void draw();

    void turnSnake();

    void handleGame();

    void moveSnake(int x, int y);
};
