#include "Game.h"

Game::Game(int newScreenWidth, int newScreenHeight) {
    screenWidth = newScreenWidth;
    screenHeight = newScreenHeight;

    snake = new Snake(CELL_SIZE * 5, CELL_SIZE * 5, START_SPEED, CELL_SIZE);
    food.push_back(new Cell(generateNumber(1, screenWidth / CELL_SIZE - 2),
                            generateNumber(2, screenHeight / CELL_SIZE - 3)));
    paused = false;
    startSnakeTimer = time(nullptr);
    startFoodTimer = time(nullptr);
    gameStatus = RUNNING;
    score = 0;

    Image bug = LoadImage("../resources/bug.png");
    bugImage = LoadTextureFromImage(bug);
    bugImage.height = CELL_SIZE;
    bugImage.width = CELL_SIZE;
    UnloadImage(bug);
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

    UnloadTexture(bugImage);
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
                if (snake->getDirection() == LEFT) {
                    break;
                }
                snake->setDirection(RIGHT);
                break;

            case KEY_LEFT:
                if (snake->getDirection() == RIGHT) {
                    break;
                }
                snake->setDirection(LEFT);
                break;

            case KEY_UP:
                if (snake->getDirection() == DOWN) {
                    break;
                }
                snake->setDirection(UP);
                break;

            case KEY_DOWN:
                if (snake->getDirection() == UP) {
                    break;
                }
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

void Game::drawBackground() const {
    std::string scoreText = "score: ";
    scoreText.append(std::to_string(score));
    DrawText(scoreText.c_str(), CELL_SIZE, CELL_SIZE, 25, DARKGRAY);

    for (int i = CELL_SIZE; i < screenWidth - CELL_SIZE; i += CELL_SIZE) {
        for (int j = CELL_SIZE * 2; j < screenHeight - CELL_SIZE; j += CELL_SIZE) {
            if (((i + j) / CELL_SIZE) % 2 == 0) {
                DrawRectangle(i, j, CELL_SIZE, CELL_SIZE, {132, 235, 80, 255});
            } else {
                DrawRectangle(i, j, CELL_SIZE, CELL_SIZE, {94, 186, 69, 255});
            }
        }
    }

    DrawRectangleLines(CELL_SIZE, CELL_SIZE * 2, screenWidth - CELL_SIZE * 2, screenHeight - CELL_SIZE * 3,
                       BLACK);
}

void Game::draw() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    drawBackground();

    std::lock_guard<std::mutex> lockSnake(snakeMutex);
    std::lock_guard<std::mutex> lockFood(foodMutex);

    for (Cell *cell: food) {
        DrawTexture(bugImage, cell->getX(), cell->getY(), WHITE);
    }

    for (Cell *cell: snake->getBody()) {
        cell->draw(CELL_SIZE, DARKBLUE);
    }

    EndDrawing();
}


void Game::moveSnake() {
    while (gameStatus == RUNNING) {

        if (paused) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            time(&startSnakeTimer);
            continue;
        }

        controlSpeed();

        if (!setSnakePosition()) {
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(snake->getSpeed()));
        cv.notify_one();
    }
}

bool Game::setSnakePosition() {
    int prevX = snake->getHead()->getX();
    int prevY = snake->getHead()->getY();

    switch (snake->getDirection()) {
        case UP:
            if (isGameOver(prevX, prevY - CELL_SIZE)) {
                return false;
            }
            snake->getHead()->setCoordinate(prevX, prevY - CELL_SIZE);
            break;
        case DOWN:
            if (isGameOver(prevX, prevY + CELL_SIZE)) {
                return false;
            }
            snake->getHead()->setCoordinate(prevX, prevY + CELL_SIZE);
            break;
        case RIGHT:
            if (isGameOver(prevX + CELL_SIZE, prevY)) {
                return false;
            }
            snake->getHead()->setCoordinate(prevX + CELL_SIZE, prevY);
            break;
        case LEFT:
            if (isGameOver(prevX - CELL_SIZE, prevY)) {
                return false;
            }
            snake->getHead()->setCoordinate(prevX - CELL_SIZE, prevY);
            break;
    }

    eatFood();

    if (isVictory()) {
        return false;
    }

    for (int i = 1; i < snake->getLength(); i++) {
        int currentX = snake->getBody()[i]->getX();
        int currentY = snake->getBody()[i]->getY();

        snake->getBody()[i]->setCoordinate(prevX, prevY);
        prevX = currentX;
        prevY = currentY;
    }
    return true;
}

void Game::generateFood() {
    while (gameStatus == RUNNING) {

        if (paused) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            time(&startFoodTimer);
            continue;
        }
        setFoodPosition();
    }
}

void Game::setFoodPosition() {
    time_t now;
    time(&now);
    if (now - startFoodTimer >= 4) {
        time(&startFoodTimer);

        int cellAmountX = (screenWidth) / CELL_SIZE - 2;
        int cellAmountY = (screenHeight) / CELL_SIZE - 3;

        int x = generateNumber(1, cellAmountX);
        int y = generateNumber(2, cellAmountY);

        while (isInArray(x, y, snake->getBody(), 0) || isInArray(x, y, food, 0)) {
            x = generateNumber(1, cellAmountX);
            y = generateNumber(2, cellAmountY);
        }

        addFood(x, y);
        cv.notify_one();
    }
}

int Game::generateNumber(int min, int max) {
    std::random_device rd;
    std::uniform_int_distribution<int> dist(min, max);
    return dist(rd) * CELL_SIZE;
}

void Game::eatFood() {
    std::lock_guard<std::mutex> lockSnake(snakeMutex);
    std::lock_guard<std::mutex> lockFood(foodMutex);

    for (int i = 0; i < food.size(); i++) {
        if ((food[i]->getX() == snake->getHead()->getX()) && (food[i]->getY() == snake->getHead()->getY())) {
            snake->grow();
            delete food[i];
            food.erase(food.begin() + i);
            score++;
            break;
        }
    }
}

bool Game::isOutOfBoundary(int a, int min, int max) {
    if (a > max || a < min) {
        return true;
    }
    return false;
}

void Game::pauseGame() {
    paused = !paused;
}

void Game::controlSpeed() {
    time_t now;
    time(&now);

    std::lock_guard<std::mutex> lockSnake(snakeMutex);

    if (now - startSnakeTimer > 15 && snake->getSpeed() > 40) {
        snake->setSpeed(snake->getSpeed() - 20);
        time(&startSnakeTimer);
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


bool Game::isGameOver(int x, int y) {
    if (isInArray(x, y, snake->getBody(), 1)
        || isOutOfBoundary(x, CELL_SIZE, screenWidth - CELL_SIZE * 2)
        || isOutOfBoundary(y, CELL_SIZE * 2, screenHeight - CELL_SIZE * 2)) {
        endGame(LOSS);
        return true;
    }
    return false;
}

GameStatus Game::getGameStatus() const {
    return gameStatus;
}

bool Game::isVictory() {
    if (snake->getLength() == screenWidth * screenHeight) {
        endGame(WON);
        return true;
    }
    return false;
}

int Game::getScore() const {
    return score;
}

Snake *Game::getSnake() const {
    return snake;
}

const std::vector<Cell *> &Game::getFood() const {
    return food;
}

bool Game::isPaused() const {
    return paused;
}

void Game::addFood(int x, int y) {
    foodMutex.lock();
    food.push_back(new Cell(x, y));
    foodMutex.unlock();
}




