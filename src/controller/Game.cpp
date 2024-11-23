#include <cstdlib>
#include <thread>
#include <iostream>
#include "Game.h"

Game::Game(int screenWidth, int screenHeight, const char *title, int startSpeed, int cellSize) {
    board = new Board(screenWidth, screenHeight, title, cellSize);
    board->initWindow();
    snake = new Snake(cellSize * 5, cellSize * 5, startSpeed, cellSize);
    food.push_back(new Cell(generateNumber(screenWidth / cellSize) * board->getCellSize(),
                            generateNumber(screenHeight / cellSize) * board->getCellSize()));
    paused = false;
    isRunning = true;
    time(&startSnake);
    time(&startFood);
}

Game::~Game() {
    for (Cell *cell: food) {
        delete cell;
    }
    delete board;
    delete snake;
}

void Game::startGame() {
    std::thread catchInput(&Game::waitForInput, this);
    std::thread moveSnake(&Game::moveSnake, this);
    std::thread generateFood(&Game::generateFood, this);

    while (isRunning && !WindowShouldClose()) {
        board->setWidth(GetScreenWidth());
        board->setHeight(GetScreenHeight());
        handleGame();
        draw();
    }

    if (WindowShouldClose()) {
        endGame();
    }

    std::lock_guard<std::mutex> lock(commandsMutex);

    catchInput.join();
    moveSnake.join();
    generateFood.join();

    CloseWindow();
}

void Game::waitForInput() {
    while (isRunning) {
        int key = GetKeyPressed();
        if (key != 0) {
            commandsMutex.lock();
            commands.push(key);
            commandsMutex.unlock();
            cv.notify_one();
        }
    }
}

void Game::handleGame() {
    while (!commands.empty()) {
        commandsMutex.lock();
        int command = commands.front();
        commands.pop();
        commandsMutex.unlock();

        switch (command) {
            case KEY_RIGHT:
                snake->setDirection(RIGHT);
                break;
            case KEY_LEFT:
                snake->setDirection(LEFT);
                break;
            case KEY_UP:
                snake->setDirection(UP);
                break;
            case KEY_DOWN:
                snake->setDirection(DOWN);
                break;
            case KEY_SPACE:
                pauseGame();
                break;
            case KEY_ESCAPE:
                endGame();
                return;
            default:
                break;
        }
    }
}

void Game::endGame() {
    isRunning = false;
    cv.notify_all();
}

void Game::draw() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    for (int i = board->getCellSize(); i < board->getHeight(); i += board->getCellSize()) {
        DrawLine(i, 0, i, board->getHeight(), LIGHTGRAY);
        DrawLine(0, i, board->getWidth(), i, LIGHTGRAY);
    }

    std::lock_guard<std::mutex> lockSnake(snakeMutex);
    std::lock_guard<std::mutex> lockFood(foodMutex);

    for (Cell *cell: food) {
        cell->draw(board->getCellSize(), GREEN);
    }

    for (Cell *cell: snake->getBody()) {
        cell->draw(board->getCellSize(), MAROON);
    }

    EndDrawing();
}


void Game::moveSnake() {
    while (isRunning) {

        if (paused) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }

        if (isGameOver()) {
            endGame();
            break;
        }

        eatFood();
        controlSpeed();

        int prevX = snake->getBody()[0]->getX();
        int prevY = snake->getBody()[0]->getY();

        switch (snake->getDirection()) {
            case UP:
                snake->getBody()[0]->setCoordinate(prevX, prevY - snake->getWidth());
                break;
            case DOWN:
                snake->getBody()[0]->setCoordinate(prevX, prevY + snake->getWidth());
                break;
            case RIGHT:
                snake->getBody()[0]->setCoordinate(prevX + snake->getWidth(), prevY);
                break;
            case LEFT:
                snake->getBody()[0]->setCoordinate(prevX - snake->getWidth(), prevY);
                break;
        }

        snake->getBody()[0]->setX(checkBoundary(snake->getBody()[0]->getX(), board->getWidth()));
        snake->getBody()[0]->setY(checkBoundary(snake->getBody()[0]->getY(), board->getHeight()));

        for (int i = 1; i < snake->getLength(); i++) {
            int currentX = snake->getBody()[i]->getX();
            int currentY = snake->getBody()[i]->getY();

            snake->getBody()[i]->setCoordinate(prevX, prevY);
            prevX = currentX;
            prevY = currentY;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(snake->getSpeed()));
        cv.notify_one();
    }
}

void Game::generateFood() {
    while (isRunning) {

        if (paused) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }

        time_t now;
        time(&now);

        if (now - startFood > 5) {
            time(&startFood);

            int cellAmount = board->getWidth() / board->getCellSize();
            int x = generateNumber(cellAmount) * board->getCellSize();
            int y = generateNumber(cellAmount) * board->getCellSize();

            std::lock_guard<std::mutex> lockSnake(snakeMutex);

            while (isInArray(x, y, snake->getBody(), 0) || isInArray(x, y, food, 0)) {
                x = generateNumber(cellAmount) * board->getCellSize();
                y = generateNumber(cellAmount) * board->getCellSize();
            }


            foodMutex.lock();
            food.push_back(new Cell(x, y));
            foodMutex.unlock();
            cv.notify_one();
        }
    }
}

int Game::generateNumber(int max) {
    return (int) random() % (max + 1);
}

void Game::eatFood() {
    std::lock_guard<std::mutex> lockSnake(snakeMutex);
    std::lock_guard<std::mutex> lockFood(foodMutex);

    for (int i = 0; i < food.size(); i++) {
        if (food[i]->getX() == snake->getBody()[0]->getX() && food[i]->getY() == snake->getBody()[0]->getY()) {
            snake->grow();
            delete food[i];
            food.erase(food.begin() + i);
            break;
        }
    }
}

int Game::checkBoundary(int a, int max) {
    if (a >= max) {
        return 0;
    } else if (a < 0) {
        return max;
    }
    return a;
}

void Game::pauseGame() {
    paused = !paused;
}

void Game::controlSpeed() {
    time_t now;
    time(&now);

    std::lock_guard<std::mutex> lockSnake(snakeMutex);

    if (now - startSnake > 15 && snake->getSpeed() > 40) {
        time(&startSnake);
        snake->setSpeed(snake->getSpeed() - 20);
    }
}

bool Game::isInArray(int x, int y, const std::vector<Cell *>& array, int startIndex) {
    for (int i = startIndex; i < array.size(); i++) {
        if (x == array[i]->getX() && y == array[i]->getY()) {
            return true;
        }
    }
    return false;
}

bool Game::isGameOver() {
    if (isInArray(snake->getBody()[0]->getX(), snake->getBody()[0]->getY(), snake->getBody(), 1)) {
        return true;
    }
    return false;
}

