#include "player.h"
#include "meteor.h"
#include "game.h"
#include "assetsMenu.h"
#include "assetsGame.h"

const int screenWidth = 1024;
const int screenHeight = 768;

bool isMenu = false;
bool exitWindows = false;
bool exitGameplay = false;
bool gameover = false;
bool pause = false;
int framesCounter = 0;

void SetupGame();
void DrawPause();
void Update();
void DrawGame();
void Menu();
void RestartPreGameplay();
void Play(Rectangle mousepos, Rectangle play);
void Exit(Rectangle mousepos, Rectangle exit);
void Credits(Rectangle mousepos, Rectangle credits);
void Rules(Rectangle mousepos, Rectangle rules);
void MenuTexts();


void RunGame()
{
    InitWindow(screenWidth, screenHeight, "Asteroids By: Manuel Dantuono");
    InitAudioDevice();
    LoadResourcesGame();
    LoadResourcesMenu();
    SetTargetFPS(60);

    

    while (!exitWindows && !WindowShouldClose())
    {
        Menu();
    }

    UnloadResourcesMenu();
    UnloadResourcesGame();
    CloseAudioDevice();
    CloseWindow();
}

void SetupGame()
{
    BeginDrawing();
    EnemySetup();
    SetupPlayer();
    SetupMeteor();
    PowerUpsSetup();
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
            PowerUpLogic();
            LogicEnemy();
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
        DrawEnemy();
        PowerUpDraw();

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
    isMenu = true;
    BackgroundMenu();

    PlayMusicMenu(isMenu);
    Rectangle mousepos = { (float)GetMouseX(), (float)GetMouseY(), 1, 1 };
    Rectangle play = { screenWidth / 2 - 150, screenHeight / 2, 300, 150 };
    Rectangle exit = { 950, 10, 50,50 };
    Rectangle credits = { screenWidth / 2 + 170, screenHeight / 2, 300, 150 };
    Rectangle rules = { screenWidth / 2 - 470, screenHeight / 2, 300, 150 };

    ClearBackground(BLACK);
    ShowCursor();

    DrawTitle();
    
    DrawOptionPlay(0);
    DrawOptionRules(0);
    DrawOptionCredits(0);
    DrawRectangleGradientEx(exit, DARKBROWN, DARKBLUE, WHITE, DARKBROWN);
    
    MenuTexts();
    Play(mousepos, play);
    Exit(mousepos, exit);
    Credits(mousepos, credits);
    Rules(mousepos, rules);
    
 
    if (IsKeyPressed(KEY_ESCAPE))
    {
        exitWindows = true;
    }

    EndDrawing();
    SetExitKey(NULL);
}

void RestartPreGameplay() {
    exitGameplay = false;
    if (pause)
    {
        pause = false;
    }
}

void MenuTexts() {
    DrawText("JUGAR", screenWidth / 2 - 95, screenHeight / 2 + 50, 60, SKYBLUE);
    DrawText("CREDITOS", screenWidth / 2 + 190, screenHeight / 2 + 50, 50, SKYBLUE);
    DrawText("REGLAS", screenWidth / 2 - 440, screenHeight / 2 + 50, 60, SKYBLUE);
    DrawText("x", 965, 15, 40, RED);
}

void Play(Rectangle mousepos, Rectangle play) {

    if (CheckCollisionRecs(mousepos, play)) {

        DrawOptionPlay(1);
        DrawText("JUGAR", screenWidth / 2 - 95, screenHeight / 2 + 50, 60, WHITE);

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        {
            PlayMusicMenu(!isMenu);
            EndDrawing();
            SetupGame();
            RestartPreGameplay();
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
        DrawOptionCredits(1);
        DrawText("CREDITOS", screenWidth / 2 + 190, screenHeight / 2 + 50, 50, WHITE);

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            EndDrawing();
            exitGameplay = false;
            framesCounter = 0;
            while (!exitGameplay)
            {
                PlayMusicMenu(isMenu);
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

                if (IsKeyDown(KEY_ENTER)) framesCounter += 8;
                else framesCounter++;

                ClearBackground(BLACK);
                DrawText(TextSubtext(message, 0, framesCounter / 5), 210, 160, 40, WHITE);
                DrawText("(SPACE) Salida Rapida.", 670, 10, 30, WHITE);
                EndDrawing();
            }
            
        }
    }
}

void Rules(Rectangle mousepos, Rectangle rules) {
    if (CheckCollisionRecs(mousepos, rules))
    {
        DrawOptionRules(1);
        DrawText("REGLAS", screenWidth / 2 - 440, screenHeight / 2 + 50, 60, WHITE);

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {

            EndDrawing();
            exitGameplay = false;
            framesCounter = 0;
            while (!exitGameplay)
            {
                PlayMusicMenu(isMenu);
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

                if (IsKeyDown(KEY_ENTER)) framesCounter += 8;
                else framesCounter++;

                ClearBackground(BLACK);
                DrawText(TextSubtext(message, 0, framesCounter / 5), 210, 160, 40, WHITE);
                DrawText("(SPACE) Salida Rapida.", 670, 10, 30, WHITE);
                EndDrawing();
            }

        }
    }
}
