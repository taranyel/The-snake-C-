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
    int screenWidth, screenHeight;

public:
    Game(int newScreenWidth, int newScreenHeight);

    ~Game();

    void startGame();

    void endGame(GameStatus status);

    void pauseGame();

    void draw();

    void drawBackground() const;

    void moveSnake();

    void controlGame();

    void generateFood();

    void eatFood();

    static int generateNumber(int min, int max);

    static bool isOutOfBoundary(int a, int min, int max);

    void waitForInput();

    void controlSpeed();

    static bool isInArray(int x, int y, const std::vector<Cell *>& array, int startIndex);

    bool isGameOver(int x, int y);

    bool isVictory();

    GameStatus getGameStatus() const;

    int getScore() const;

    Snake *getSnake() const;

    const std::vector<Cell *> &getFood() const;

    bool isPaused() const;

    void addFood(int x, int y);

    bool setSnakePosition();

    void setFoodPosition();
};
