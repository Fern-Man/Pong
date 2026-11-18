#include <iostream>
#include <raylib.h>

struct Ball{
    float x = 0, y = 0;
    int radius = 20;
    int speedX = 7;
    int speedY = 7;
};
struct Paddle{
    float x = 0, y = 0;
    float width = 25;
    float height = 120;
    float speed = 6;
};

void limitMovement(float x, float &y, float height){
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
    int speedChoices[2] = {-1, 1};

    ball.speedX *= speedChoices[GetRandomValue(0, 1)];
    ball.speedY *= speedChoices[GetRandomValue(0, 1)];
}
void ballUpdate(Ball &ball, int &playerScore, int &cpuScore){
    ball.x += ball.speedX;
    ball.y += ball.speedY;

    if(ball.y + ball.radius <= 0 || ball.y + ball.radius >= GetScreenHeight()){
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
}
void playerUpdate(Paddle &player){
    if(IsKeyDown(KEY_UP)){
        player.y -= player.speed;
    }
    else if(IsKeyDown(KEY_DOWN)){
        player.y += player.speed;
    }

    limitMovement(player.x, player.y, player.height);
}
void cpuUpdate(Paddle &cpu, Ball &ball){  //cpu = opponent ai, not actual cpu
    if(cpu.y + cpu.height/2 > ball.y){
        cpu.y -= cpu.speed;
    }
    else if(cpu.y + cpu.height/2 <= ball.y){
        cpu.y += cpu.speed;
    }

    limitMovement(cpu.x, cpu.y, cpu.height);
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
    std::cout << "GAME: CPU setup complete\n\n"; //two new lines for cleaner output

    InitWindow(screenWidth, screenHeight, "Pong");
    SetTargetFPS(60);

    std::cout << "\nGAME: Starting the game...\n"; //new line before for cleaner output
    while(WindowShouldClose() == false){
        BeginDrawing();
        ClearBackground(BLACK);

        ballUpdate(ball, playerScore, cpuScore);
        playerUpdate(player);
        cpuUpdate(cpu, ball);

        if(CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{player.x, player.y, player.width, player.height})){
            ball.speedX = -abs(ball.speedX);
        }
        else if(CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{cpu.x, cpu.y, cpu.width, cpu.height})){
            ball.speedX = abs(ball.speedX);
        }

        DrawLine(screenWidth/2, 0, screenWidth/2, screenHeight, WHITE);
        DrawCircle(ball.x, ball.y, ball.radius, WHITE);
        DrawRectangle(player.x, player.y, player.width, player.height, WHITE);
        DrawRectangle(cpu.x, cpu.y, cpu.width, cpu.height, WHITE);
        DrawText(TextFormat("%i", cpuScore), screenWidth/4 - 20, 20, 80, WHITE);
        DrawText(TextFormat("%i", playerScore), 3 * screenWidth/4 - 20, 20, 80, WHITE);

        EndDrawing();
    }

    std::cout << "GAME: Closing the game...\n\n";
    CloseWindow();
    return 0;
