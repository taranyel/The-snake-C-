#include <queue>
#include <mutex>
#include <condition_variable>
#include "../entity/Snake.h"

class Game {
private:
    Snake *snake;
    std::mutex snakeMutex, foodMutex, commandsMutex;
    Board *board;
    std::vector<Cell *> food;
    std::queue<int> commands;
    bool paused, isRunning;
    std::condition_variable cv;
    std::time_t startSnake, startFood;

public:
    Game(int screenWidth, int screenHeight, const char *title, int startSpeed, int cellSize);

    ~Game();

    void startGame();

    void endGame();

    void pauseGame();

    void draw();

    void moveSnake();

    void handleGame();

    void generateFood();

    void eatFood();

    static int generateNumber(int max);

    int checkBoundary(int a, int max);

    void waitForInput();

    void controlSpeed();

    bool isInArray(int x, int y, const std::vector<Cell *>& array, int startIndex);

    bool isGameOver();
};
