#include <queue>
#include <mutex>
#include <cstdlib>
#include <thread>
#include <iostream>
#include <random>
#include <condition_variable>
#include "../entity/Snake.h"
#include "../entity/Cell.h"
#include "GameStatus.h"

#define CELL_SIZE 30
#define START_SPEED 150

class Game {
private:
    Snake *snake;
    std::mutex snakeMutex, foodMutex, commandsMutex;
    std::vector<Cell *> food;
    std::queue<int> commands;
    bool paused;
    std::condition_variable cv;
    std::time_t startSnakeTimer, startFoodTimer;
    GameStatus gameStatus;
    int score;
    Texture2D bugImage;

public:
    Game();

    ~Game();

    void startGame();

    void endGame(GameStatus status);

    void pauseGame();

    void draw();

    void drawBackground();

    void moveSnake();

    void controlGame();

    void generateFood();

    void eatFood();

    static int generateNumber(int min, int max);

    int checkBorder(int a, int min, int max, bool xCoord);

    void waitForInput();

    void controlSpeed();

    bool isInArray(int x, int y, const std::vector<Cell *>& array, int startIndex);

    bool isGameOver();

    bool isVictory();

    GameStatus getGameStatus() const;

    int getScore() const;
};
