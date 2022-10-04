#include "player.h"
#include "meteor.h"
#include "powerUps.h"
#include "game.h"
#include "assetsMenu.h"
#include "assetsGame.h"
#include "propellerAnimated.h"

const int screenWidth = 1024;
const int screenHeight = 768;

bool isMenu = false;
bool exitWindowRequested = false;
bool exitWindows = false;
bool exitGameplay = false;
bool gameover = false;
bool pause = false;
bool isOnOption = false;

void SetupGame(bool isVictory);
void DrawPause();
void Update();
void DrawGame();
void Menu();
void RestartPreGameplay();
void DrawScoreMenu();

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

void SetupGame(bool isVictory)
{
    BeginDrawing();
    SetupMeteor(isVictory);
    PropellerSetup();
    EnemySetup();
    SetupPlayer();
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
            SetupGame(0);
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
    { DrawText("Nivel Completado!!", screenWidth / 2 - MeasureText("Nivel Completado!!", 20) / 2 - 130, screenHeight / 2 - 50, 50, GREEN);
    DrawText("(Enter) Para continuar", screenWidth / 2 - MeasureText("(Enter) Para continuar", 20) / 2 - 150, screenHeight / 2 , 50, GREEN);
    }

    
        if (IsKeyPressed(KEY_ENTER) && victory == true)
        {
            SetupGame(1);
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
    DrawScoreMenu();
    PlayMusicMenu(isMenu);
    Rectangle mousepos = { (float)GetMouseX(), (float)GetMouseY(), 1, 1 };
    Rectangle play = { screenWidth / 2 - 150, screenHeight / 2, 300, 150 };
    Rectangle exit = { 950, 10, 50,50 };
    Rectangle credits = { screenWidth / 2 + 170, screenHeight / 2, 300, 150 };
    Rectangle rules = { screenWidth / 2 - 470, screenHeight / 2, 300, 150 };
    Rectangle options = { screenWidth / 2 - 150, screenHeight / 2 + 170, 300, 150 };

    ClearBackground(BLACK);
    ShowCursor();

    DrawTitle();
    
    DrawOptionPlay(0);
    DrawOptionRules(0);
    DrawOptionCredits(0);
    DrawOptions(0);
    DrawRectangleGradientEx(exit, WHITE, DARKBLUE, WHITE, DARKBLUE);
    
    MenuTexts();
    Play(mousepos, play);
    Exit(mousepos, exit);
    Credits(mousepos, credits);
    Rules(mousepos, rules);
    Options(mousepos, options);

    if (IsKeyPressed(KEY_ESCAPE))
    {
        exitWindowRequested = true;
    }

    if (exitWindowRequested)
    {
        DrawText("Seguro que quieres salir? [Y/N]", screenWidth / 2 - 400, screenHeight / 2 - 70, 50, WHITE);
        if (IsKeyPressed(KEY_Y)) exitWindows = true;
        else if (IsKeyPressed(KEY_N)) exitWindowRequested = false;
    }
    EndDrawing();
    SetExitKey(NULL);
}

void RestartPreGameplay() {
    exitGameplay = false;
    exitWindowRequested = false;
    if (pause)
    {
        pause = false;
    }
}

void MenuTexts() {
    DrawText("JUGAR", screenWidth / 2 - 95, screenHeight / 2 + 50, 60, SKYBLUE);
    DrawText("OPCIONES", screenWidth / 2 - 130, screenHeight / 2 + 250, 50, SKYBLUE);
    DrawText("CREDITOS", screenWidth / 2 + 190, screenHeight / 2 + 50, 50, SKYBLUE);
    DrawText("REGLAS", screenWidth / 2 - 440, screenHeight / 2 + 50, 60, SKYBLUE);
    DrawText("x", 965, 15, 40, BLACK);
}

void Play(Rectangle mousepos, Rectangle play) {

    if (CheckCollisionRecs(mousepos, play)) {

        DrawOptionPlay(1);
        DrawText("JUGAR", screenWidth / 2 - 95, screenHeight / 2 + 50, 60, WHITE);

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        {
            PlayMusicMenu(!isMenu);
            EndDrawing();
            SetupGame(0);
            RestartPreGameplay();
            while (!exitGameplay && !WindowShouldClose())
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
            exitWindowRequested = true;
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
            exitWindowRequested = false;
            EndDrawing();
            exitGameplay = false;
            framesCounter = 0;
            while (!exitGameplay && !WindowShouldClose())
            {
                PlayMusicMenu(isMenu);
                int finalmessage = 10000;
                const char message[700] = "Juego Creado por Manuel Dantuono \nTexturas de Asteroides, Nave y Disparos hechos por Manuel Dantuono\n\nSonido de Juego: ( Close Combat ) por Fredrik Ekstrom. \nwww.epidemicsound.com/track/vnourn9aT7/9\nSonido del Menu: (Fugent) por Lupus Nocte \nwww.epidemicsound.com/track/cAEBqYRFz0/  \nSonido de impacto 1: (Bomb Explosion 3) \nwww.epidemicsound.com/track/ZATPJMOSHa/ \nSonido de impacto 2: (Storefront Crash 1) \nwww.epidemicsound.com/track/lC2s9JHZqR/ \nImagen del Menu: \nclipart-library.com/clipart/space-free-png-image.htm \nImagen dentro del Gameplay: \nopengameart.org/content/space-parallax-background \n\n\n\nJuego hecho con Libreria Raylib.";
                
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
                else framesCounter += 4;

                ClearBackground(BLACK);
                DrawText(TextSubtext(message, 0, framesCounter / 5), 1, 1, 27, WHITE);
                DrawText("(SPACE) Salida Rapida | (Enter) Mas velocidad.", 550, 1, 20, WHITE);
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
            exitWindowRequested = false;
            newImage = 1;
            EndDrawing();
            exitGameplay = false;
           
            while (!exitGameplay && !WindowShouldClose())
            {
                PlayMusicMenu(isMenu);
                BeginDrawing();
                ClearBackground(BLACK);
                if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))
                {
                    newImage++;
                }

                DrawRules(newImage);
                if (newImage > 3)
                {
                    exitGameplay = true;
                }
                
                EndDrawing();
            }

        }
    }
}

void Options(Rectangle mousepos, Rectangle options) {
    if (CheckCollisionRecs(mousepos, options))
    {
        DrawOptions(1);
        DrawText("OPCIONES", screenWidth / 2 - 130, screenHeight / 2 + 250, 50, WHITE);

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {     
            exitWindowRequested = false;
            EndDrawing();
            
            exitGameplay = false;
            while (!exitGameplay && !WindowShouldClose())
            {
                
                ClearBackground(BLACK);
                DrawText("(SPACE) Salir", 800, 1, 30, WHITE);
                PlayMusicMenu(isMenu);             
                Rectangle mouseposition = { (float)GetMouseX(), (float)GetMouseY(), 1, 1 };
                Rectangle fullscren = { screenWidth / 2 - 240, screenHeight / 2 - 270, 500, 100 };
                Rectangle defaultscreen = { screenWidth / 2 - 240, screenHeight / 2 - 70, 500, 100 };

                BeginDrawing();

                DrawText("Resolucion de Pantalla", screenWidth / 2 - 280, screenHeight / 2 - 350, 50, WHITE);

                if (CheckCollisionRecs(mouseposition, fullscren) && !isOnOption)
                {
                    DrawRectangleRec(fullscren, RED);
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    {
                        isOnOption = true;
                        ToggleFullscreen();
                    }
                }
                else if (isOnOption)
                {
                    DrawRectangleRec(fullscren, GREEN);
                }else
                {
                    DrawRectangleRec(fullscren, WHITE);
                }



                
                if (CheckCollisionRecs(mouseposition, defaultscreen) && isOnOption)
                {
                    DrawRectangleRec(defaultscreen, RED);
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    {
                        isOnOption = false;
                        ToggleFullscreen();
                    }
                }
                else if (!isOnOption)
                {
                    DrawRectangleRec(defaultscreen, GREEN);
                }
                else
                {
                    DrawRectangleRec(defaultscreen, WHITE);
                }
                
          
                DrawText("FULLSCREEN", screenWidth / 2 - 150, screenHeight / 2 - 250, 50, BLACK);
                DrawText("1024 X 768", screenWidth / 2 - 125, screenHeight / 2 - 50, 50, BLACK);



               

                if (IsKeyPressed(KEY_SPACE))
                {
                    exitGameplay = true;
                }
                EndDrawing();
            }

        }
    }
}
