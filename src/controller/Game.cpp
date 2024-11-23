#include "Game.h"

Game::Game() {
    snake = new Snake(CELL_SIZE * 5, CELL_SIZE * 5, START_SPEED, CELL_SIZE);
    food.push_back(new Cell(generateNumber(GetScreenWidth() / CELL_SIZE) * CELL_SIZE,
                            generateNumber(GetScreenHeight() / CELL_SIZE) * CELL_SIZE));
    paused = false;
    time(&startSnakeTimer);
    time(&startFoodTimer);
    gameStatus = RUNNING;
}

Game::~Game() {
    for (Cell *cell: food) {
        delete cell;
    }
    delete snake;
}

void Game::startGame() {
    std::thread catchInput(&Game::waitForInput, this);
    std::thread moveSnake(&Game::moveSnake, this);
    std::thread generateFood(&Game::generateFood, this);

    while (gameStatus == RUNNING) {

        if (WindowShouldClose()) {
            cv.notify_all();
            gameStatus = QUIT;
            break;
        }

        controlGame();
        draw();
    }

    std::lock_guard<std::mutex> lock(commandsMutex);

    catchInput.join();
    moveSnake.join();
    generateFood.join();
}

void Game::waitForInput() {
    while (gameStatus == RUNNING) {
        int key = GetKeyPressed();
        if (key != 0) {
            commandsMutex.lock();
            commands.push(key);
            commandsMutex.unlock();
            cv.notify_one();
        }
    }
}

void Game::controlGame() {
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
            case KEY_Q:
                endGame(QUIT);
                break;
            default:
                break;
        }
    }
}

void Game::endGame(GameStatus status) {
    cv.notify_all();
    gameStatus = status;
}

void Game::draw() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    for (int i = CELL_SIZE; i < GetScreenHeight(); i += CELL_SIZE) {
        DrawLine(i, 0, i, GetScreenHeight(), LIGHTGRAY);
        DrawLine(0, i, GetScreenWidth(), i, LIGHTGRAY);
    }

    std::lock_guard<std::mutex> lockSnake(snakeMutex);
    std::lock_guard<std::mutex> lockFood(foodMutex);

    for (Cell *cell: food) {
        cell->draw(CELL_SIZE, GREEN);
    }

    for (Cell *cell: snake->getBody()) {
        cell->draw(CELL_SIZE, MAROON);
    }

    EndDrawing();
}


void Game::moveSnake() {
    while (gameStatus == RUNNING) {

        if (paused) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }

        if (isGameOver()) {
            endGame(LOSS);
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

        snake->getBody()[0]->setX(checkBoundary(snake->getBody()[0]->getX(), GetScreenWidth()));
        snake->getBody()[0]->setY(checkBoundary(snake->getBody()[0]->getY(), GetScreenHeight()));

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
    while (gameStatus == RUNNING) {

        if (paused) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }

        time_t now;
        time(&now);

        if (now - startFoodTimer > 5) {
            time(&startFoodTimer);

            int cellAmountX = GetScreenWidth() / CELL_SIZE;
            int cellAmountY = GetScreenHeight() / CELL_SIZE;

            int x = generateNumber(cellAmountX) * CELL_SIZE;
            int y = generateNumber(cellAmountY) * CELL_SIZE;

            std::lock_guard<std::mutex> lockSnake(snakeMutex);

            while (isInArray(x, y, snake->getBody(), 0) || isInArray(x, y, food, 0)) {
                x = generateNumber(cellAmountX) * CELL_SIZE;
                y = generateNumber(cellAmountY) * CELL_SIZE;
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

    if (now - startSnakeTimer > 15 && snake->getSpeed() > 40) {
        time(&startSnakeTimer);
        snake->setSpeed(snake->getSpeed() - 20);
    }
}

bool Game::isInArray(int x, int y, const std::vector<Cell *> &array, int startIndex) {
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