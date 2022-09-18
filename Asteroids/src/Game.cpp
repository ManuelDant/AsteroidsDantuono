#include "raylib.h"
#include "player.h"
#include "meteor.h"
#include <cmath>

const int screenWidth = 1024;
const int screenHeight = 768;

void SetupGame();
void Update();
void DrawGame(); 
void DrawPause();

void RunGame() {
    InitWindow(screenWidth, screenHeight, "Asteroids By: Manuel Dantuono");
    SetTargetFPS(60);

    SetupGame();

    while (!WindowShouldClose())
    {
        Update();
        DrawGame();
    }

    CloseWindow();
}

void SetupGame()
{
    SetupPlayer();
    SetupMeteor();
    
}

void DrawGame() {
    BeginDrawing();

    ClearBackground(RAYWHITE);

    DrawPause();

    EndDrawing();

}

void Update()
{
    if (!gameover)
    {
        if (IsKeyPressed('P')) pause = !pause;

        if (!pause)
        {
          
            MovePlayer();
            ColisionWall();
            ColisionMeteors();
            LogicMeteor();

            if (destroyedMeteorsCount == maxBigMeteors + maxMidMeteors + maxSmallMeteors) victory = true;
        }
        else
        {
            if (IsKeyPressed(KEY_ENTER))
            {
                SetupGame();
                gameover = false;
            }
        }
    }
}

void DrawPause() {
    if (!gameover)
    {
        PlayerDraw();
        DrawMeteors();

        if (victory) DrawText("VICTORY", screenWidth / 2 - MeasureText("VICTORY", 20) / 2, screenHeight / 2, 20, LIGHTGRAY);

        if (pause) DrawText("GAME PAUSED", screenWidth / 2 - MeasureText("GAME PAUSED", 40) / 2, screenHeight / 2 - 40, 40, GRAY);
    }
    else DrawText("PRESS [ENTER] TO PLAY AGAIN", GetScreenWidth() / 2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20) / 2, GetScreenHeight() / 2 - 50, 20, GRAY);
}
