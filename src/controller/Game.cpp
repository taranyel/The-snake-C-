#include <cstdlib>
#include <thread>
#include <iostream>
#include "Game.h"

Game::Game(int screenWidth, int screenHeight, const char *title, int startSpeed, int cellSize) {
    board = new Board(screenWidth, screenHeight, title, cellSize);
    board->initWindow();
    snake = new Snake(cellSize * 5, cellSize * 5, startSpeed, cellSize);
    food = new Cell(generateNumber(screenWidth / cellSize) * cellSize,
                    generateNumber(screenHeight / cellSize) * cellSize);
    paused = false;
    isRunning = true;
}

Game::~Game() {
    delete food;
    delete board;
    delete snake;
}

void Game::startGame() {
    std::thread catchInput(&Game::waitForInput, this);
    std::thread move(&Game::moveSnake, this);

    while (!WindowShouldClose()) {
        board->setWidth(GetScreenWidth());
        board->setHeight(GetScreenHeight());
        handleGame();
        draw();
    }

    std::lock_guard<std::mutex> lock(commandsMutex);
    isRunning = false;
    cv.notify_all();

    catchInput.join();
    move.join();
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
                isRunning = false;
                cv.notify_all();
                endGame();
                return;
            default:
                break;
        }
    }
}

void Game::endGame() {
    CloseWindow();
}

void Game::draw() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    for (int i = board->getCellSize(); i < board->getHeight(); i += board->getCellSize()) {
        DrawLine(i, 0, i, board->getHeight(), LIGHTGRAY);
        DrawLine(0, i, board->getWidth(), i, LIGHTGRAY);
    }

    food->draw(board->getCellSize(), GREEN);

    for (Cell *cell: snake->getBody()) {
        cell->draw(board->getCellSize(), MAROON);
    }
    EndDrawing();
}


void Game::moveSnake() {
    while (isRunning) {
        if (paused) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Avoid busy-waiting
            continue;
        }

        snakeMutex.lock();
        eatFood();

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

        snakeMutex.unlock();

        std::this_thread::sleep_for(std::chrono::milliseconds(snake->getSpeed()));
        cv.notify_one();
    }
}

void Game::generateFood() {
    int cellAmount = board->getWidth() / board->getCellSize();
    int x = generateNumber(cellAmount) * board->getCellSize();
    int y = generateNumber(cellAmount) * board->getCellSize();

    for (int i = 0; i < snake->getLength(); i++) {
        if (x == snake->getBody()[i]->getX() || y == snake->getBody()[i]->getY()) {
            x = generateNumber(cellAmount) * board->getCellSize();
            y = generateNumber(cellAmount) * board->getCellSize();
            i = 0;
        }
    }

    delete food;
    food = new Cell(x, y);
}

int Game::generateNumber(int max) {
    return (int) random() % (max + 1);
}

void Game::eatFood() {
    if (food->getX() == snake->getBody()[0]->getX() && food->getY() == snake->getBody()[0]->getY()) {
        snake->grow();
        generateFood();
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

