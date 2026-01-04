#pragma once
#include <deque>
#include "Game.hpp"
#include "include/raylib/raylib.h"
#include "include/raylib/raymath.h"
class Food{
private:
    Texture2D texture;
public:
    Vector2 position;

    Food(std::deque<Vector2> snakeBody);

    Vector2 generateRandCell();

    Vector2 generateRandPos(std::deque<Vector2> snakeBody);
    
    void draw();

    ~Food();
};