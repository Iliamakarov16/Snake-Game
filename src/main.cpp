#include <iostream>
#include <deque>
#include "include/raylib/raylib.h"
#include "include/raylib/raymath.h"

//custom colors
Color green = {173, 204, 96, 255};

//grid system
const int cellSize = 32; 
const int cellCount = 25;

bool isGameActive = false;
bool shouldAddSegment = false;
bool elementInDeque(Vector2 element, std::deque<Vector2> deck){
    for(auto segment : deck){
        if (Vector2Equals(element, segment)){
            return true;
        }
    } 
    return false;
}
//Commented code will be implemented in the future
class Snake{
private:
    //snake sprite logic
    Texture2D textureSheet;
    enum SpriteIndex{
        HEAD_L, HEAD_D, HEAD_R, HEAD_U,
        BODY_H, BODY_V,
        TURN_U_R, TURN_U_L, TURN_D_R, TURN_D_L,
        TAIL_R, TAIL_U, TAIL_L, TAIL_D 
    };
    Rectangle spriteRect(int index) const{
        return Rectangle{(float)(index * cellSize), 0.0f, (float)cellSize, (float)cellSize};
    }
    //checks head direction
    int headIndex() const{
        if (direction.x == 1) return HEAD_R;
        if (direction.x == -1) return HEAD_L;
        if (direction.y == -1) return HEAD_U;
        return HEAD_D;
    }
    //checks tail direction 
    int tailIndex() const{
        Vector2 tail = body.back();
        Vector2 beforeTail = body[body.size() - 2];
        Vector2 dir = Vector2Subtract(tail, beforeTail);

        if (dir.x == 1) return TAIL_R;
        if (dir.x == -1) return TAIL_L;
        if (dir.y == -1) return TAIL_U;
        return TAIL_D;
    }
    //checks body segment direction
    int bodyIndex(size_t i) const{
        Vector2 prev = body[i - 1];
        Vector2 curr = body[i];
        Vector2 next = body[i + 1];
        Vector2 dirPrev = Vector2Subtract(prev, curr);
        Vector2 dirNext = Vector2Subtract(next, curr);

        if (dirPrev.x == dirNext.x) return BODY_V;
        if (dirPrev.y == dirNext.y) return BODY_H;

        bool up = (dirPrev.y == -1) || (dirNext.y == -1);
        bool down = (dirPrev.y == 1) || (dirNext.y == 1);
        bool left = (dirPrev.x == -1) || (dirNext.x == -1);
        bool right = (dirPrev.x == 1) || (dirNext.x == 1);

        if (up && right) return TURN_U_R;
        if (up && left) return TURN_U_L;
        if (down && right) return TURN_D_R;
        return TURN_D_L;
    }
    
public:
    std::deque<Vector2> body = {Vector2{21, 0}, Vector2{22, 0}, Vector2{23, 0}};
    Vector2 direction = {-1, 0};

    Snake(){
        //texture load
        textureSheet = LoadTexture("graphics/SnakeSpriteSheet.png");
    }

    void draw(){
        for (size_t i = 0; i < body.size(); ++i){
            Vector2 segment = body[i];
            Vector2 pos = {segment.x * cellSize, segment.y * cellSize};
            int index = BODY_H;
            if (i == 0){
                index = headIndex();
            }
            else if (i == body.size() - 1){
                index = tailIndex();
            }
            else{
                index = bodyIndex(i);
            }
            DrawTextureRec(textureSheet, spriteRect(index), pos, WHITE);
        }
    }

    void update(){
        body.push_front(Vector2Add(body.front(), direction));
        if (shouldAddSegment){      
            shouldAddSegment = false;
        }
        else{
            body.pop_back();
        }
        
        
    }
    //takes input for direction
    void move(){
        if ((IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) && direction.y != 1){
            direction = {0, -1};
            isGameActive = true;            
        }
        if ((IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) && direction.y != -1){
            direction = {0, 1};
            isGameActive = true;
        }
        if ((IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) && direction.x != 1){
            direction = {-1, 0};
            isGameActive = true;                        
        }
        if ((IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) && direction.x != -1){
            direction = {1, 0};
            isGameActive = true;                            
        }
    }

    void reset(){
        body = {Vector2{21, 0}, Vector2{22, 0}, Vector2{23, 0}};
        direction = {-1, 0};    
    }

    ~Snake(){
       UnloadTexture(textureSheet);
    }
};

class Food{
private:
    Texture2D texture;
public:

    Vector2 position;

    Food(std::deque<Vector2> snakeBody){
        //texture load
        Image image = LoadImage("graphics/apple.png");
        texture = LoadTextureFromImage(image);
        UnloadImage(image);

        position = generateRandPos(snakeBody);
    }

    Vector2 generateRandCell(){
        float x = GetRandomValue(0, cellCount - 1);
        float y = GetRandomValue(0, cellCount - 1);

        return Vector2{x, y};
    }

    Vector2 generateRandPos(std::deque<Vector2> snakeBody){
        Vector2 newPosition = generateRandCell();
        while(elementInDeque(newPosition, snakeBody)){
            newPosition = generateRandCell();
        }
        return newPosition;
    }
    
    void draw(){
        DrawTexture(texture, position.x * cellSize, position.y * cellSize, WHITE);
    }

    ~Food(){
       UnloadTexture(texture);
    }
};

class Game{
public:
    Snake snake = Snake();
    Food food = Food(snake.body);

    //returns true if time passed >= interval parameter. (used to update snake every interval instead of every frame)
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
            checkCollisionWithFood();
            checkCollisionWithEdges();
            checkCollisionWithBody();
        }
        snake.move();
        
        if (IsKeyPressed(KEY_R)){//pause
            isGameActive = false;
        }
    }
    void checkCollisionWithFood(){
        if (Vector2Equals(snake.body.front(), food.position)){
            food.position = food.generateRandPos(snake.body);
            shouldAddSegment = true;
        }
    }
    void checkCollisionWithEdges(){
        if (snake.body.front().y < 0 || snake.body.front().y >= cellCount){
            gameOver();
        }
        if (snake.body.front().x < 0 || snake.body.front().x >= cellCount){
            gameOver();
        }
    }

    void checkCollisionWithBody(){
        std::deque<Vector2> deque = snake.body;
        deque.pop_front();
        if (elementInDeque(snake.body.front(), deque)){
            gameOver();
        }
    }

    void gameOver(){
        isGameActive = false;
        snake.reset();
        food.position = food.generateRandPos(snake.body);
    }

};
int main(){ 
    //Window setup
    InitWindow(cellSize * cellCount, cellSize * cellCount, "Snake Game");
    SetTargetFPS(60);

    const char *app_dir = GetApplicationDirectory();
    ChangeDirectory(app_dir);

    Game game;

    while(!WindowShouldClose()){
        BeginDrawing();
        //Update
        game.update();
        //Draw
        ClearBackground(green);
        game.draw();

        EndDrawing();
    }

    CloseWindow();
}