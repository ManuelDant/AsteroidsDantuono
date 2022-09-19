#include "raylib.h"
#include "player.h"
#include "meteor.h"
#include "game.h"
#include <cmath>

const int screenWidth = 1024;
const int screenHeight = 768;

static bool gameover = false;
static bool pause = false;
static bool victory = false;

void SetupGame();
void DrawPause();
void Update();
void DrawGame();



void RunGame()
{
    InitWindow(screenWidth, screenHeight, "Asteroids By: Manuel Dantuono");
    
    SetTargetFPS(60);

    SetupGame();

    while (!WindowShouldClose())
    {
        Update();
        //DrawTexture(texture, screenWidth / 2 - texture.width / 2, screenHeight / 2 - texture.height / 2, WHITE);
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

    ClearBackground(GRAY);

    DrawPause();

    EndDrawing();

}

void DefeatPlayer()
{
    gameover = true;

}

void Restart() {
    if (!pause && gameover)
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            SetupGame();
            gameover = false;
        }
    }
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

            if (IsKeyDown(KEY_X))
            {
                gameover = true;
            }

        }
    }
}


void DrawPause() {
    if (!gameover)
    {
        PlayerDraw();
        DrawMeteors();

        if (victory) DrawText("VICTORIA!!", screenWidth / 2 - MeasureText("VICTORIA!!", 20) / 2, screenHeight / 2, 50, DARKPURPLE);

        if (pause) DrawText("Pausa! (P para reanudar)", screenWidth / 2 - MeasureText("Pausa! (P para reaunar)", 40) / 2 - 100, screenHeight / 2 - 40, 60, DARKPURPLE);
    }
    else DrawText("Presiona (ENTER) para volver a jugar", GetScreenWidth() / 2 - MeasureText("Presiona (ENTER) para volver a jugar", 20) / 2 - 300, GetScreenHeight() / 2 - 50, 50, RED);
    Restart();
}
