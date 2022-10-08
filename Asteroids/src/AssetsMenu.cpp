#include "assetsMenu.h"

#include <string>

const int screenWidth = 1024;
const int screenHeight = 768;

void LoadResourcesMenu() {
    title = LoadTexture("rsc/title.png");
    bground = LoadTexture("rsc/bground.png");
    option = LoadTexture("rsc/option.png");
    menubg = LoadMusicStream("rsc/menubg.mp3");
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

void CreditsOptions() {
    Rectangle mouseposition = { (float)GetMouseX(), (float)GetMouseY(), 1, 1 };
    Rectangle SoundCombat = { screenWidth / 2 - 240, screenHeight / 2 - 370, 500, 100 };
    Rectangle MenuSound = { screenWidth / 2 - 240, screenHeight / 2 - 220, 500, 100 };
    Rectangle ImpactSound = { screenWidth / 2 - 240, screenHeight / 2 - 70, 500, 100 };
    Rectangle ImpactSound2 = { screenWidth / 2 - 240, screenHeight / 2 + 80, 500, 100 };
    Rectangle MenuImage = { screenWidth / 2 - 240, screenHeight / 2 + 230, 500, 100 };
    Rectangle GameplayImage = { screenWidth - 200, screenHeight / 2 - 70, 180, 100, };
    Rectangle OwnAssets = { 30, screenHeight / 2 - 70, 180, 100, };

    std::string soundCombat = " start https://www.epidemicsound.com/track/vnourn9aT7/9";
    std::string menuSound = " start https://www.epidemicsound.com/track/cAEBqYRFz0/";
    std::string impactSound = " start https://www.epidemicsound.com/track/ZATPJMOSHa/";
    std::string impactSound2 = " start https://www.epidemicsound.com/track/lC2s9JHZqR/";
    std::string menuImage = " start http://clipart-library.com/clipart/space-free-png-image.htm";
    std::string gameplayImage = " start https://opengameart.org/content/space-parallax-background";

    BeginDrawing();

    DrawRectangleRec(SoundCombat, WHITE);
    DrawRectangleRec(MenuSound, WHITE);
    DrawRectangleRec(ImpactSound, WHITE);
    DrawRectangleRec(ImpactSound2, WHITE);
    DrawRectangleRec(MenuImage, WHITE);
    DrawRectangleRec(GameplayImage, WHITE);
    DrawRectangleRec(OwnAssets, GREEN);


    if (CheckCollisionRecs(mouseposition, SoundCombat))
    {
        DrawRectangleRec(SoundCombat, RED);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            system(soundCombat.c_str());
        }
    }

    if (CheckCollisionRecs(mouseposition, MenuSound))
    {
        DrawRectangleRec(MenuSound, RED);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            system(menuSound.c_str());
        }
    }
    if (CheckCollisionRecs(mouseposition, ImpactSound))
    {
        DrawRectangleRec(ImpactSound, RED);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            system(impactSound.c_str());
        }
    }
    if (CheckCollisionRecs(mouseposition, ImpactSound2))
    {
        DrawRectangleRec(ImpactSound2, RED);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            system(impactSound2.c_str());
        }
    }
    if (CheckCollisionRecs(mouseposition, MenuImage))
    {
        DrawRectangleRec(MenuImage, RED);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            system(menuImage.c_str());
        }
    }
    if (CheckCollisionRecs(mouseposition, GameplayImage))
    {
        DrawRectangleRec(GameplayImage, RED);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            system(gameplayImage.c_str());
        }
    }
}

void CreditsText() {
    DrawText("Close Combat por Fredrik Ekstrom", screenWidth / 2 - 200, screenHeight / 2 - 340, 25, BLACK);
    DrawText("Fugent por Lupus Nocte", screenWidth / 2 - 125, screenHeight / 2 - 190, 25, BLACK);
    DrawText("Bomb Explosion 3 por Epidemic Sound", screenWidth / 2 - 215, screenHeight / 2 - 40, 25, BLACK);
    DrawText("Storefront Crash por Epidemic Sound", screenWidth / 2 - 220, screenHeight / 2 + 110, 25, BLACK);
    DrawText("Space Image por Clipart Library", screenWidth / 2 - 190, screenHeight / 2 + 260, 25, BLACK);
    DrawText("Space Parallax", screenWidth - 185, screenHeight / 2 - 50, 20, BLACK);
    DrawText("por Scorcher24", screenWidth - 190, screenHeight / 2 - 10, 20, BLACK);
    DrawText("Botones, Nave y", 35, screenHeight / 2 - 65, 20, BLACK);
    DrawText("Asteroides por", 45, screenHeight / 2 - 35, 20, BLACK);
    DrawText("Manuel Dantuono", 35, screenHeight / 2 - 5, 20, BLACK);
}