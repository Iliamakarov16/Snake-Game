#pragma once
#include <deque>
#include "Game.hpp"
#include "include/raylib/raylib.h"
#include "include/raylib/raymath.h"

class Snake{
// private:
//     Texture2D textureHead;
//     Texture2D textureBody;
//     Texture2D textureTail;
public:
    std::deque<Vector2> body;
    Vector2 direction;
    Game game;

    Snake();

    void draw();
   
    void update();

    void move();
    
};