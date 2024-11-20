#include <queue>
#include <mutex>
#include <condition_variable>
#include "../entity/Snake.h"

class Game {
private:
    Snake *snake;
    std::mutex snakeMutex;
    Board *board;
    Cell *food;
    std::queue<int> commands;
    std::mutex commandsMutex;
    bool paused;
    bool isRunning;
    std::condition_variable cv;

public:
    Game(int screenWidth, int screenHeight, const char *title, int startSpeed, int cellSize);

    ~Game();

    void startGame();

    static void endGame();

    void pauseGame();

    void draw();

    void moveSnake();

    void handleGame();

    void generateFood();

    void eatFood();

    static int generateNumber(int max);

    int checkBoundary(int a, int max);

    void waitForInput();
};
