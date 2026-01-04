#include "Snake.hpp"

Snake::Snake(){
    body = {Vector2{21, 0}, Vector2{22, 0}, Vector2{23, 0}};
    direction = {0, 0};
    // //texture load
    // Image imageHead = LoadImage("sprites/SnakeHead.png");
    // Image imageBody = LoadImage("sprites/SnakeBody.png");
    // Image imageTail = LoadImage("sprites/SnakeTail.png");

    // textureHead = LoadTextureFromImage(imageHead);
    // textureBody = LoadTextureFromImage(imageBody);
    // textureTail = LoadTextureFromImage(imageTail);

    // UnloadImage(imageHead);
    // UnloadImage(imageBody);
    // UnloadImage(imageTail);
}

// void draw(){
//     for (auto segment : body){
//         // if (segment == body.front()){
//         //     DrawTexture(textureHead, segment.x * cellSize, segment.y * cellSize, WHITE);
//         // }
//         // else if (segment == body.back()){
//         //     DrawTexture(textureTail, segment.x * cellSize, segment.y * cellSize, WHITE);
//         // }
//         // else{
//         //     DrawTexture(textureBody, segment.x * cellSize, segment.y * cellSize, WHITE); 
//         // }
//     }
// }

// ~Snake(){
//    UnloadTexture(textureHead);
//    UnloadTexture(textureBody);
//    UnloadTexture(textureTail);
// }

void Snake::draw(){
    for (auto segment : body){
        Rectangle segmentrect = {segment.x * game.cellSize, segment.y * game.cellSize, game.cellSize, game.cellSize};
        DrawRectangleRounded(segmentrect, 0.5, 3, game.darkGreen);
    }
}

void Snake::update(){
    body.push_front(Vector2Add(body.front(), direction));
    if (game.shouldAddSegment){      
        game.shouldAddSegment = false;
    }
    else{
        body.pop_back();
    }
    
    
}

void Snake::move(){
    if ((IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) && direction.y != 1){
        direction = {0, -1};
        game.isGameActive = true;            
    }
    if ((IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) && direction.y != -1){
        direction = {0, 1};
        game.isGameActive = true;
    }
    if ((IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) && direction.x != 1){
        direction = {-1, 0};
        game.isGameActive = true;                        
    }
    if ((IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) && direction.x != -1){
        direction = {1, 0};
        game.isGameActive = true;                            
    }
}