#include "raylib.h"
#include "player.h"
#include <cmath>

const int screenWidth = 1024;
const int screenHeight = 768;

static bool gameover = false;
static bool pause = false;
static bool victory = false;

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
 
        }

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

void DrawPause() {
    if (!gameover)
    {
        PlayerDraw();
        if (pause) DrawText("GAME PAUSED", screenWidth / 2 - MeasureText("GAME PAUSED", 40) / 2, screenHeight / 2 - 40, 40, GRAY);
    }
    else DrawText("PRESS [ENTER] TO PLAY AGAIN", GetScreenWidth() / 2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20) / 2, GetScreenHeight() / 2 - 50, 20, GRAY);
}