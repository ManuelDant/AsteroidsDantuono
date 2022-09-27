#include "assetsMenu.h"

void LoadResourcesMenu() {
    title = LoadTexture("rsc/title.png");
    bground = LoadTexture("rsc/bground.png");
    option = LoadTexture("rsc/option.png");
    menubg = LoadMusicStream("menubg.mp3");
    imgEnemy = LoadTexture("rsc/imageEnemy.png");
    imgPowerUps = LoadTexture("rsc/imagePowerUps.png");
    controls = LoadTexture("rsc/controls.png");

    SetMusicVolume(menubg, -0.05f);

    bground.width = 1500;
    bground.height = 400;
    option.width = 500;
    option.height = 500;

}

void UnloadResourcesMenu() {
    UnloadTexture(option);
    UnloadMusicStream(menubg);
    UnloadTexture(title);
    UnloadTexture(bground);
    UnloadTexture(imgEnemy);
    UnloadTexture(imgPowerUps);
    UnloadTexture(controls);
}

void PlayMusicMenu(bool isOn) {
    if (isOn)
    {
        PlayMusicStream(menubg);
        UpdateMusicStream(menubg);
    }
    else
    {
        StopMusicStream(menubg);
    }
   
}

void DrawOptionPlay(bool isOn) {

    if (!isOn)
    {
        DrawTexture(option, GetScreenWidth() / 2 - 260, GetScreenHeight() / 2 - 210, WHITE);
    }
    else
    {
        DrawTexture(option, GetScreenWidth() / 2 - 260, GetScreenHeight() / 2 - 210, RED);
    }

}

void DrawOptionRules(bool isOn) {
    if (!isOn)
    {
        DrawTexture(option, GetScreenWidth() / 2 - 580, GetScreenHeight() / 2 - 210, WHITE);
    }
    else
    {
        DrawTexture(option, GetScreenWidth() / 2 - 580, GetScreenHeight() / 2 - 210, RED);
    }
}

void DrawOptionCredits(bool isOn) {
    if (!isOn)
    {
        DrawTexture(option, GetScreenWidth() / 2 + 60, GetScreenHeight() / 2 - 210, WHITE);
    }
    else
    {
        DrawTexture(option, GetScreenWidth() / 2 + 60, GetScreenHeight() / 2 - 210, RED);
    }
}

void DrawOptions(bool isOn) {
    if (!isOn)
    {
        DrawTexture(option, GetScreenWidth() / 2 - 260, GetScreenHeight() / 2 - 10, WHITE);
    }
    else
    {
        DrawTexture(option, GetScreenWidth() / 2 - 260, GetScreenHeight() / 2 - 10, RED);
    }
}

void DrawTitle() {
    title.width = 1000;
    title.height = 700;
    DrawTexture(title, GetScreenWidth() / 2 - 500, GetScreenHeight() / 2 - 480, WHITE);
}

void BackgroundMenu() {

    scrollingBackMenu -= 1.0f;
    if (scrollingBackMenu <= -bground.width * 2 + 1050) scrollingBackMenu = 0;
    DrawTextureEx(bground, { scrollingBackMenu }, 0.0f, 2.0f, WHITE);
    DrawTextureEx(bground, { bground.width + scrollingBackMenu }, 0.0f, 0.0f, WHITE);
}

void DrawRules(int framesCounter) {
    switch (framesCounter)
    {
    case 1:
        imgEnemy.width = 1050;
        imgEnemy.height = 768;
        DrawTexture(imgEnemy, 1, 1, WHITE);
        DrawText("(ENTER O SPACE) para continuar", GetScreenWidth() / 2 - 100 , GetScreenHeight() / 2 - 240, 30, GREEN);
        break;
    case 2:
        imgPowerUps.width = 1050;
        imgPowerUps.height = 768;
        DrawTexture(imgPowerUps, 1, 1, WHITE);
        break;
    case 3:
        controls.width = 1050;
        controls.height = 768;
        DrawTexture(controls, 1, 1, WHITE);
        break;
    }
}