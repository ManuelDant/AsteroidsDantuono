#include "raylib.h"
#include "player.h"
#include "meteor.h"
#include "game.h"
#include <cmath>

const int screenWidth = 1024;
const int screenHeight = 768;

static bool gameover = false;
static bool pause = false;

void SetupGame();
void DrawPause();
void Update();
void DrawGame();
void Menu();

void RunGame()
{
    InitWindow(screenWidth, screenHeight, "Asteroids By: Manuel Dantuono");
    InitAudioDevice();
    
    LoadResources();

    SetTargetFPS(60);

    

    while (!WindowShouldClose())
    {
        Menu();
    }

    
    UnloadResources();
    CloseAudioDevice();
    CloseWindow();
}

void SetupGame()
{
    BeginDrawing();
    SetupPlayer();
    SetupMeteor();
    EndDrawing();
    
}

void DrawGame() {
    BeginDrawing();

    ClearBackground(BLACK);

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

void Victory(bool victory) {

    if (victory) 
    { DrawText("VICTORIA!!", screenWidth / 2 - MeasureText("VICTORIA!!", 20) / 2, screenHeight / 2, 50, LIGHTGRAY); }

    
        if (IsKeyPressed(KEY_ENTER) && victory == true)
        {
            SetupGame();
            gameover = false;
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

        Victory(0);

        if (pause) DrawText("Pausa! (P para reanudar)", screenWidth / 2 - MeasureText("Pausa! (P para reaunar)", 40) / 2 - 150, screenHeight / 2 - 40, 60, DARKPURPLE);
    }
    else DrawText("Presiona (ENTER) para volver a jugar", GetScreenWidth() / 2 - MeasureText("Presiona (ENTER) para volver a jugar", 20) / 2 - 300, GetScreenHeight() / 2 - 50, 50, RED);
    Restart();
}

void Menu() {
    Rectangle mousepos;
    Rectangle play;
    mousepos = { (float)GetMouseX(), (float)GetMouseY(), 1, 1 };
    play = { screenWidth / 2 - 150, screenHeight / 2, 300, 150 };

    ClearBackground(BLACK);
    ShowCursor();

    
    DrawText("Asteoirds", screenWidth / 2 - 200, screenHeight / 2 - 300, 100, RED);
    DrawRectangleGradientEx(play, ORANGE, WHITE, WHITE, ORANGE);
    DrawText("JUGAR", screenWidth / 2 - 50, screenHeight / 2 + 50, 30, RED);
    DrawRectangleRec(mousepos, RED);

    if (CheckCollisionRecs(mousepos, play)) {

        DrawRectangleRec(play, RED);
        DrawText("JUGAR", screenWidth / 2 - 50, screenHeight / 2 + 50, 30, WHITE);

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        {
            EndDrawing();
            SetupGame();
            while (!WindowShouldClose())
            {
                Update();
                DrawGame();
            }

            gameover = false;
        }
    }
 
    
    EndDrawing();
}
