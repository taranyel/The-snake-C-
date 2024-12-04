# Snake

## Project description

Snake is a classic arcade-style game where the player controls a growing snake that moves around the screen. The goal is
typically to eat food items to make the snake grow longer while avoiding collisions with the screen boundaries or the
snake's own body.

## Gameplay

### Controls

The player uses arrow keys to control the direction of the snake's movement: up, down, left, or right.
Once the snake moves in a direction, it cannot reverse direction immediately (e.g., if it's moving right, it can't turn
directly left).

### Objective

Eat "food" (bugs) that appears randomly on the screen.
Each piece of food consumed increases the length of the snake.

### Challenges

- <b>Self-Collision:</b> The snake's body grows longer as it eats food, and the player must avoid running into their own tail.
- <b>Wall Collision:</b> Hitting the edges of the play area ends the game.
- <b>Speed:</b> The snake's speed increases as the game progresses, making it harder to control.


## Build

Type these commands to command prompt to build the game:

```
cd build
rm -rf build
cmake ..
cmake --build .
```

## Run

Type these commands to command prompt to run the game:

```
./snake
```

## Fix problems

### Problem
```
CMake Error at CMakeLists.txt:1 (cmake_minimum_required):
CMake 3.20 or higher is required.  You are running version 3.19
```

### Solution

Update your CMake to latest version or if it is not working, change 
CMake version to yours at CMakeLists.txt:1, rebuild and rerun the game.