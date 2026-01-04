#pragma once
#include "Snake.hpp"
#include "Food.hpp"
#include "include/raylib/raylib.h"
#include "include/raylib/raymath.h"

class Snake;
class Food;

class Game{
public:
    //custom colors
    Color green = {173, 204, 96, 255};
    Color darkGreen = {43, 51, 24, 255};

    int cellSize, cellCount;

    bool isGameActive;
    bool shouldAddSegment;

    Snake snake;
    Food food(snake.body);

    Game();
    Game(int cell_size, int cell_count);
   
    double lastUpdateTime = 0;
    bool eventTriggered(double interval);

    void draw();
    void update();

    void checkCollision();
};
