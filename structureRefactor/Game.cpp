#include "Game.hpp"
double lastUpdateTime = 0;
bool eventTriggered(double interval){
    double currTime = GetTime();

    if (currTime - lastUpdateTime >= interval){
        lastUpdateTime = currTime;
        return true;
    }
    return false;
} 

void draw(){
    snake.draw();
    food.draw();
}

void update(){
    if (eventTriggered(0.2) && isGameActive){
        snake.update();
        checkCollision();
        std::cout << food.position.x << " " << food.position.y << std::endl;
    }
    snake.move();
    
    if (IsKeyPressed(KEY_R)){//pause
        isGameActive = false;
    }
}
void checkCollision(){
    if (Vector2Equals(snake.body.front(), food.position)){
        food.position = food.generateRandPos(snake.body);
        shouldAddSegment = true;
    }
}