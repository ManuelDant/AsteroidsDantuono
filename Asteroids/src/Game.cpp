#include "raylib.h"
#include "player.h"
#include "meteor.h"
#include "game.h"
#include <cmath>

const int screenWidth = 1024;
const int screenHeight = 768;

bool exitWindows = false;
bool exitGameplay = false;
static bool gameover = false;
static bool pause = false;
int framesCounter = 0;

void SetupGame();
void DrawPause();
void Update();
void DrawGame();
void Menu();
void RestarPreGameplay();
void Play(Rectangle mousepos, Rectangle play);
void Exit(Rectangle mousepos, Rectangle exit);
void Credits(Rectangle mousepos, Rectangle credits);
void MenuTexts();

void RunGame()
{
    InitWindow(screenWidth, screenHeight, "Asteroids By: Manuel Dantuono");
    InitAudioDevice();
    LoadResources();
    SetTargetFPS(60);

    

    while (!exitWindows && !WindowShouldClose())
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
        if (IsKeyPressed(KEY_SPACE))
        {
            SetupGame();
            gameover = false;
        }

        if (IsKeyPressed(KEY_ESCAPE))
        {
            exitGameplay = true;
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
    SetExitKey(KEY_NULL);
    if (!gameover)
    {
        if (IsKeyPressed(KEY_ESCAPE)) pause = !pause; 

        if (!pause)
        {
          
            MovePlayer();
            ColisionWall();
            ColisionMeteors();
            LogicMeteor();   

        }
    }
}

void DrawPause() {
    if (!gameover)
    {
        
        PlayerDraw();
        DrawMeteors();

        Victory(0);

        if (pause) DrawText("Pausa! (ESC para reanudar | SPACE para salir)", screenWidth / 2 - MeasureText("Pausa! (ESC para reanudar | SPACE para salir)", 5) / 2 - 250, screenHeight / 2 - 40, 30, DARKPURPLE);
    }
    else {
        DrawText("Presiona (SPACE) para volver a jugar", GetScreenWidth() / 2 - MeasureText("Presiona (SPACE) para volver a jugar", 20) / 2 - 300, GetScreenHeight() / 2 - 50, 50, RED);
        DrawText("Para salir presione (ESC)", screenWidth / 2 - 50, screenHeight / 2 + 340, 40, RED);
    }
    Restart();
}

void Menu() {
    

    Rectangle mousepos = { (float)GetMouseX(), (float)GetMouseY(), 1, 1 };
    Rectangle play = { screenWidth / 2 - 150, screenHeight / 2, 300, 150 };
    Rectangle exit = { 950, 10, 50,50 };
    Rectangle credits = { screenWidth / 2 + 170, screenHeight / 2, 300, 150 };

    ClearBackground(BLACK);
    ShowCursor();
    DrawRectangleRec(mousepos, RED);

    DrawRectangleGradientEx(play, ORANGE, WHITE, WHITE, ORANGE);
    DrawRectangleGradientEx(exit, ORANGE, WHITE, WHITE, ORANGE);
    DrawRectangleGradientEx(credits, ORANGE, WHITE, WHITE, ORANGE);
  
    MenuTexts();
    Play(mousepos, play);
    Exit(mousepos, exit);
    Credits(mousepos, credits);
    
 
    if (IsKeyReleased(KEY_ESCAPE))
    {
        exitWindows = true;
    }

    EndDrawing();
    SetExitKey(KEY_ESCAPE);
}

void RestarPreGameplay() {

    exitGameplay = false;
    if (pause)
    {
        pause = false;
    }
}

void MenuTexts() {
    DrawText("Asteroids", screenWidth / 2 - 200, screenHeight / 2 - 300, 100, RED);
    DrawText("JUGAR", screenWidth / 2 - 50, screenHeight / 2 + 50, 30, RED);
    DrawText("CREDITOS", screenWidth / 2 + 240, screenHeight / 2 + 50, 30, RED);
    DrawText("x", 965, 15, 40, RED);

}

void Play(Rectangle mousepos, Rectangle play) {

    if (CheckCollisionRecs(mousepos, play)) {

        DrawRectangleRec(play, RED);
        DrawText("JUGAR", screenWidth / 2 - 50, screenHeight / 2 + 50, 30, WHITE);

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        {
            EndDrawing();
            SetupGame();
            RestarPreGameplay();
            while (!exitGameplay)
            {
                Update();
                DrawGame();

                if (pause)
                {
                    if (IsKeyDown(KEY_SPACE))
                    {
                        exitGameplay = true;
                    }
                }
            }

            gameover = false;
        }
    }
}

void Exit(Rectangle mousepos, Rectangle exit) {
    if (CheckCollisionRecs(mousepos,exit))
    {
        DrawRectangleRec(exit,RED);
        DrawText("x", 965, 15, 40, WHITE);

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            exitWindows = true;
        }
    }
}

void Credits(Rectangle mousepos, Rectangle credits) {
    if (CheckCollisionRecs(mousepos, credits))
    {
        DrawRectangleRec(credits, RED);
        DrawText("CREDITOS", screenWidth / 2 + 240, screenHeight / 2 + 50, 30, WHITE);

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            EndDrawing();
            exitGameplay = false;
            framesCounter = 0;
            while (!exitGameplay)
            {
                int finalmessage = 1000;
                const char message[128] = "Testeando el mensaje...";
                
                if (IsKeyDown(KEY_SPACE))
                {
                    framesCounter = finalmessage + 1;
                }
                if (framesCounter > finalmessage)
                {
                    exitGameplay = true;
                }
                BeginDrawing();
                
                if (IsKeyDown(KEY_SPACE)) framesCounter += 8;
                else framesCounter++;
                
                ClearBackground(BLACK);
                DrawText(TextSubtext(message, 0, framesCounter / 5), 210, 160, 40, WHITE);
                DrawText("(SPACE) Salida Rapida.", 670, 10, 30, WHITE);
                EndDrawing();
            }
            
        }
    }
}
