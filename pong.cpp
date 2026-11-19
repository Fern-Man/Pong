#include <iostream>
#include <raylib.h>

struct Ball{
    float x = 0, y = 0;
    int radius = 20;
    const int defaultSpeed = 7;
    int speedX = defaultSpeed;
    int speedY = defaultSpeed;
    std::string direction;
};
struct Paddle{
    float x = 0, y = 0;
    float width = 25;
    float height = 120;
    float roundness = 0.8;
    int segment = 0;
    float speed = 6;
    std::string direction;
};


void limitMovement(float &y, float height){
    if(y <= 0){
        y = 0;
    }
    else if(y + height >= GetScreenHeight()){
        y = GetScreenHeight() - height; 
    }
}

void resetBall(Ball &ball){
    ball.x = GetScreenWidth()/2;
    ball.y = GetScreenHeight()/2;
    ball.speedX = ball.defaultSpeed;
    ball.speedY = ball.defaultSpeed;
    int speedChoices[2] = {-1, 1};

    ball.speedX *= speedChoices[GetRandomValue(0, 1)];
    ball.speedY *= speedChoices[GetRandomValue(0, 1)];
}

void ballUpdate(Ball &ball, int &playerScore, int &cpuScore){
    const int MAX_SPEED = 15;
    ball.x += ball.speedX;
    ball.y += ball.speedY;

    if(ball.speedY < 0){
        ball.direction = "UP";
    }
    else if(ball.speedY > 0){
        ball.direction = "DOWN";
    }

    if(ball.y - ball.radius <= 0 || ball.y + ball.radius >= GetScreenHeight()){
        ball.speedY *= -1;
    }
    else if(ball.x + ball.radius >= GetScreenWidth()){
        cpuScore++;
        resetBall(ball);
    }
    else if(ball.x - ball.radius <= 0){
        playerScore++;
        resetBall(ball);
    }

    if(ball.speedX < -MAX_SPEED){
        ball.speedX = -(MAX_SPEED - 1);
    }
    else if(ball.speedY < -MAX_SPEED){
        ball.speedY = -(MAX_SPEED - 1);
    }
    else if(ball.speedX > MAX_SPEED){
        ball.speedX = MAX_SPEED - 1; 
    }
    else if(ball.speedY > MAX_SPEED){
        ball.speedY = MAX_SPEED - 1; 
    }
    else if(ball.speedX == 0){
        ball.speedX = ball.defaultSpeed;
    }
    else if(ball.speedY == 0){
        ball.speedY = ball.defaultSpeed;
    }
}

void playerUpdate(Paddle &player){ 
    if(IsKeyDown(KEY_UP)){
        player.y -= player.speed;
        player.direction = "UP";
    }
    else if(IsKeyDown(KEY_DOWN)){
        player.y += player.speed;
        player.direction = "DOWN";
    }

    limitMovement(player.y, player.height);
}

void cpuUpdate(Paddle &cpu, const Ball &ball){  //cpu = opponent ai, not actual cpu
    if(cpu.y + cpu.height/2 > ball.y){
        cpu.y -= cpu.speed;
        cpu.direction = "UP";
    }
    else if(cpu.y + cpu.height/2 <= ball.y){
        cpu.y += cpu.speed;
        cpu.direction = "DOWN";
    } 

    limitMovement(cpu.y, cpu.height);
}

void spinTech(const Paddle &paddle, Ball &ball){
    const float SPEEDx_MULTIPLIER = 1.3;
    const float SPEEDx_DEMULTIPLIER = 0.8;
    const float SPEEDy_MULTIPLIER = 1.2;
    const float SPEEDy_DEMULTIPLIER = 0.9;

    if(ball.direction == paddle.direction){
            ball.speedX = SPEEDx_MULTIPLIER * -ball.speedX;
            ball.speedY = SPEEDy_MULTIPLIER * ball.speedY;
        }
        else if(ball.direction == "UP" && paddle.direction == "DOWN"){
            ball.speedX = SPEEDx_DEMULTIPLIER * -ball.speedX;
            ball.speedY = SPEEDy_DEMULTIPLIER * ball.speedY;
        }
        else if(ball.direction == "DOWN" && paddle.direction == "UP"){
            ball.speedX = SPEEDx_DEMULTIPLIER * -ball.speedX;
            ball.speedY = SPEEDy_DEMULTIPLIER * ball.speedY;
        }
        else{
            ball.speedX = -ball.speedX;
            ball.speedY = ball.speedY;
        }
}

void ballPaddleCollision(const Paddle &paddle, Ball &ball){
    if(CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{paddle.x, paddle.y, paddle.width, paddle.height})){
        spinTech(paddle, ball);

        if (ball.speedX > 0) {
            ball.x = paddle.x + paddle.width + ball.radius;
        } 
        else{
            ball.x = paddle.x - ball.radius;
        }
    } 
}


int main(){
    std::cout << "GAME: Initializing...\n";

    const int screenWidth = 1200;
    const int screenHeight = 700;
    int playerScore = 0;
    int cpuScore = 0;

    Ball ball;
    ball.x = screenWidth/2;
    ball.y = screenHeight/2;
    std::cout << "GAME: Ball setup complete\n";

    Paddle player;
    player.x = screenWidth - player.width - 10;
    player.y = screenHeight/2 - player.height/2;
    std::cout << "GAME: Player setup complete\n";

    Paddle cpu; 
    cpu.x = 10;
    cpu.y = screenHeight/2 - cpu.height/2;
    std::cout << "GAME: CPU setup complete\n"; 
                                                 
    Color green = Color{38, 185, 154, 255};
    Color darkGreen = Color{20, 160, 133, 255};
    Color lightGreen = Color{129, 204, 184, 255};
    Color yellow = Color{243, 213, 91, 255};
    std::cout << "GAME: Color setup complete\n\n"; //two new lines for cleaner output
 
    InitWindow(screenWidth, screenHeight, "Pong");
    SetTargetFPS(60);
    std::cout << "\nGAME: Starting the game...\n"; //new line before for cleaner output

    //game loop
    while(WindowShouldClose() == false){
        BeginDrawing();
        ClearBackground(darkGreen); 

        //update
        ballUpdate(ball, playerScore, cpuScore);
        playerUpdate(player);
        cpuUpdate(cpu, ball);
        //std::cout << ball.direction << ", " << player.direction << ", " << cpu.direction << '\n';
        std::cout << ball.speedX << ", " << ball.speedY << '\n';

        ballPaddleCollision(player, ball);
        ballPaddleCollision(cpu, ball);

        //draw
        DrawRectangle(screenWidth/2, 0, screenWidth/2, screenHeight, green);
        DrawCircle(screenWidth/2, screenHeight/2, 150, lightGreen);
        DrawLine(screenWidth/2, 0, screenWidth/2, screenHeight, WHITE);
        DrawCircle(ball.x, ball.y, ball.radius, yellow);
        DrawRectangleRounded(Rectangle{player.x, player.y, player.width, player.height}, player.roundness, player.segment, WHITE);
        DrawRectangleRounded(Rectangle{cpu.x, cpu.y, cpu.width, cpu.height}, cpu.roundness, cpu.segment, WHITE);
        DrawText(TextFormat("%i", cpuScore), screenWidth/4 - 20, 20, 80, WHITE);
        DrawText(TextFormat("%i", playerScore), 3 * screenWidth/4 - 20, 20, 80, WHITE);
        
        EndDrawing();
    }
    
    std::cout << "GAME: Closing the game...\n\n";
    CloseWindow();
    return 0;
}
