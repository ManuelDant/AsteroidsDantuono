#include "propellerAnimated.h"
#include "player.h"
#include "powerUps.h"
#include "meteor.h"
#include "assetsGame.h"

#include "raymath.h"
#include <fstream>
#include <cmath>
using namespace std;

bool isDebug = false;

void LoadResourcesGame() {
    meteorTexture = LoadTexture("rsc/meteor.png");
    ship = LoadTexture("rsc/ship.png");
    mira = LoadTexture("rsc/mira.png");
    bgroundgame = LoadTexture("rsc/bgroundgame.png");
    shipShoot = LoadSound("rsc/shoot.mp3");
    meteorImpact = LoadSound("rsc/meteorImpact.mp3");
    shipCrash = LoadSound("rsc/shipCrash.mp3");
    background = LoadMusicStream("rsc/music.mp3");
    shootexture = LoadTexture("rsc/shoot.png");
    SetSoundVolume(meteorImpact, -0.05f);
    SetSoundVolume(shipCrash, -0.05f);
    SetSoundVolume(shipShoot, static_cast<float>(-0.3));
    SetMusicVolume(background, -0.1f);
    LoadResourcePropeller();
}

void UnloadResourcesGame() {
    UnloadSound(shipShoot);
    UnloadSound(meteorImpact);
    UnloadSound(shipCrash);
    UnloadMusicStream(background);
    UnloadTexture(ship);
    UnloadTexture(meteorTexture);
    UnloadTexture(mira);
    UnloadTexture(bgroundgame);
    UnloadResourcePropeller();
}

bool CheckColissionsCircles(float c1x, float c1y, float c2x, float c2y, float c1r, float c2r) {
    float distX = c1x - c2x;
    float distY = c1y - c2y;
    float distance = sqrt((distX * distX) + (distY * distY));

    if (distance <= c1r + c2r) {
        return true;
    }
    return false;
}

void BackgroundGame() {
    scrollingBack -= 5.0f;

    if (scrollingBack <= -bgroundgame.width * 2) scrollingBack = 0;
}


//LOGIC PLAYER
void PlayerDraw() {
    int framewidth = ship.width;
    int frameheight = ship.height;
   
    Rectangle sourceRec = { 5.0f,5.0f, static_cast<float>(framewidth),static_cast<float>(frameheight) };
    Rectangle destRec = { player.position.x, player.position.y, 200, 200 };
    Vector2 Origin = { static_cast<float>(framewidth),static_cast<float>(frameheight )};


    DrawTextureEx(bgroundgame, { scrollingBack }, 0.0f, 2.0f, WHITE);
    DrawTextureEx(bgroundgame, { bgroundgame.width * 2 + scrollingBack }, 0.0f, 2.0f, WHITE);

    if (!powerUp2.active && checkPower && powerUp2.lifeSpawn > 0) {
        DrawTexturePro(ship, sourceRec, destRec, Origin, player.rotation, YELLOW);
    }
    else
    {
        DrawTexturePro(ship, sourceRec, destRec, Origin, player.rotation, WHITE);
    }
    PlayMusicStream(background);


    if (!powerUp.active && checkPower && powerUp.lifeSpawn > 0) {
        for (int i = 0; i < maxShoots; i++)
        {
            if (shoot[i].active) {
                DrawTexture(shootexture, static_cast<int>(shoot[i].position.x - 50), static_cast<int>(shoot[i].position.y - 50), GREEN);
                DrawTexture(shootexture, static_cast<int>(shoot[i].position.x - 50), static_cast<int>(shoot[i].position.y - 50), GREEN);
            }
        }
    }
    else
    {
        for (int i = 0; i < maxShoots; i++)
        {
            if (shoot[i].active) {
                DrawTexture(shootexture, static_cast<int>(shoot[i].position.x - 50), static_cast<int>(shoot[i].position.y - 50), WHITE);
            }
        }
    }
    DrawPropeller(player);
    DrawScore();
    HideCursor();
    DrawTexturePro(mira, { 5.0f,5.0f, static_cast<float>(mira.width),static_cast<float>(mira.height) }, { static_cast<float>(GetMouseX() - 17),static_cast<float>(GetMouseY() - 5), 250,250 }, { static_cast<float>(mira.width),static_cast<float>(mira.height) }, 0, WHITE);

    if (isDebug)
    {
        DrawCircle(static_cast<int>(player.position.x), static_cast<int>(player.position.y), 17, RED);
    }
}

void SetupPlayer() {
    StopMusicStream(background);
    shipHeight = (playerBaseSize / 2) / tanf(20 * DEG2RAD);

    player.position = { static_cast<float>(GetScreenWidth() / 2), static_cast<float>(GetScreenHeight() / 2 )};
    player.speed = { 0, 0 };
    player.accelerationPlayer = { 0, 0 };
    player.rotation = 0;
    player.color = LIGHTGRAY;


    for (int i = 0; i < maxShoots; i++)
    {
        shoot[i].position = { static_cast<float>(GetScreenWidth() / 2), static_cast<float>(GetScreenHeight() / 2 )};
        shoot[i].speed = { 0, 0 };
        shoot[i].radius = 4;
        shoot[i].active = false;
        shoot[i].lifeSpawn = 0;
        shoot[i].color = WHITE;
    }
}

void MovePlayer() {

    LogicPlayer();
    LogicShoot();

}

void ColisionWall() {

    if (player.position.x > GetScreenWidth() + shipHeight) player.position.x = -(shipHeight);
    else if (player.position.x < -(shipHeight)) player.position.x = GetScreenWidth() + shipHeight;
    if (player.position.y > (GetScreenHeight() + shipHeight)) player.position.y = -(shipHeight);
    else if (player.position.y < -(shipHeight)) player.position.y = GetScreenHeight() + shipHeight;
}

void LogicPlayer() {

    float angulosradianes = atan2(static_cast<float>(GetMouseY()) - player.position.y, static_cast<float>(GetMouseX()) - player.position.x );
    float angulogrados = (180 / PI) * angulosradianes - 270;

    BackgroundGame();
    UpdateMusicStream(background);

    player.rotation = angulogrados;

    float VectorDireccionX = (GetMouseX() - player.position.x);
    float VectorDireccionY = (GetMouseY() - player.position.y);
    Vector2 VectorNormalizado = Vector2Normalize({ VectorDireccionX,VectorDireccionY });

    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
    {
        player.accelerationPlayer.x += VectorNormalizado.x * 3;
        player.accelerationPlayer.y += VectorNormalizado.y * 3;

       
    }
    else
    {
        player.accelerationPlayer = player.accelerationPlayer;
    }

    player.position.x += player.accelerationPlayer.x * GetFrameTime();
    player.position.y += player.accelerationPlayer.y * GetFrameTime();
   
}


//LOGIC SHOOT
void LogicShoot() {

    if (!powerUp.active && checkPower && powerUp.lifeSpawn > 0)
    {

        powerUp.lifeSpawn++;
        if (IsKeyDown(KEY_SPACE) || IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {   
                for (int i = 0; i < maxShoots; i++)
                {
                    if (!shoot[i].active)
                    {
                        PlaySound(shipShoot);
                        shoot[i].position = { player.position.x + sin(player.rotation * DEG2RAD) * (shipHeight), player.position.y - cos(player.rotation * DEG2RAD) * (shipHeight) };
                        shoot[i].active = true;
                        shoot[i].speed.x = static_cast<float>(1.5 * sin(player.rotation * DEG2RAD) * shootSpeed);
                        shoot[i].speed.y = static_cast<float>(1.5 * cos(player.rotation * DEG2RAD) * shootSpeed);
                        shoot[i].rotation = player.rotation;
                        break;
                    }
                }
            

        }
    }
    if (IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        for (int i = 0; i < maxShoots - 10; i++)
        {
            if (!shoot[i].active)
            {
                PlaySoundMulti(shipShoot);
                shoot[i].position = { player.position.x + sin(player.rotation * DEG2RAD) * (shipHeight), player.position.y - cos(player.rotation * DEG2RAD) * (shipHeight) };
                shoot[i].active = true;
                shoot[i].speed.x = static_cast<float>(1.5 * sin(player.rotation * DEG2RAD) * shootSpeed);
                shoot[i].speed.y = static_cast<float>(1.5 * cos(player.rotation * DEG2RAD) * shootSpeed);
                shoot[i].rotation = player.rotation;
                break;
            }
        }
    }

    for (int i = 0; i < maxShoots; i++)
    {
        if (shoot[i].active) shoot[i].lifeSpawn++;
    }


    for (int i = 0; i < maxShoots; i++)
    {
        if (shoot[i].active)
        {
            if (destroyedMeteorsCount == setupBigmeteor + setupMidmeteor + setupSmallmeteor) {
                if (CheckColissionsCircles(enemy[i].position.x, enemy[i].position.y, shoot[i].position.x, shoot[i].position.y, 19, shoot[i].radius))
                {
                    PlaySoundMulti(meteorImpact);
                    shoot[i].active = false;
                    shoot[i].lifeSpawn = 0;
                    enemy[i].active = false;

                }
            }

            shoot[i].position.x += shoot[i].speed.x + 40 * GetFrameTime();
            shoot[i].position.y -= shoot[i].speed.y + 40 * GetFrameTime();


            if (shoot[i].position.x > GetScreenWidth() + shoot[i].radius)
            {
                shoot[i].active = false;
                shoot[i].lifeSpawn = 0;
            }
            else if (shoot[i].position.x < 0 - shoot[i].radius)
            {
                shoot[i].active = false;
                shoot[i].lifeSpawn = 0;
            }
            if (shoot[i].position.y > GetScreenHeight() + shoot[i].radius)
            {
                shoot[i].active = false;
                shoot[i].lifeSpawn = 0;
            }
            else if (shoot[i].position.y < 0 - shoot[i].radius)
            {
                shoot[i].active = false;
                shoot[i].lifeSpawn = 0;
            }


            if (shoot[i].lifeSpawn >= 60)
            {
                shoot[i].position = { 0, 0 };
                shoot[i].speed = { 0, 0 };
                shoot[i].lifeSpawn = 0;
                shoot[i].active = false;
            }
        }
    }


}


//LOGIC ASTEROIDS
void SetupMeteor(bool isVictory) {
    float posx;
    float posy;
    float velx;
    float vely;
    bool correctRange = false;

    if (isVictory)
    {
        setupBigmeteor += 1;
        setupMidmeteor += 2;
        setupSmallmeteor += 4;
        level++;
    }
    if (!isVictory)
    {
        setupBigmeteor = 2;
        setupMidmeteor = 4;
        setupSmallmeteor = 8;
        level = 1;
    }
    for (int i = 0; i < setupBigmeteor; i++)
    {
        posx = static_cast<float>(GetRandomValue(0, GetScreenWidth()));

        while (!correctRange)
        {
            if (posx > GetScreenWidth() / 2 - 150 && posx < GetScreenWidth() / 2 + 150) posx = static_cast<float>(GetRandomValue(0, GetScreenWidth()));
            else correctRange = true;
        }

        correctRange = false;

        posy = static_cast<float>(GetRandomValue(0, GetScreenHeight()));

        while (!correctRange)
        {
            if (posy > GetScreenHeight() / 2 - 150 && posy < GetScreenHeight() / 2 + 150)  posy = static_cast<float>(GetRandomValue(0, GetScreenHeight()));
            else correctRange = true;
        }

        bigMeteor[i].position = { posx, posy };

        correctRange = false;
        velx = static_cast<float>(GetRandomValue(-speedMeteros, speedMeteros));
        vely = static_cast<float>(GetRandomValue(-speedMeteros, speedMeteros));

        while (!correctRange)
        {
            if (velx == 0 && vely == 0)
            {
                velx = static_cast<float>(GetRandomValue(-speedMeteros, speedMeteros));
                vely = static_cast<float>(GetRandomValue(-speedMeteros, speedMeteros));
            }
            else correctRange = true;
        }

        bigMeteor[i].speed = { velx, vely };
        bigMeteor[i].radius = 55;
        bigMeteor[i].active = true;
    }

    for (int i = 0; i < setupMidmeteor; i++)
    {
        mediumMeteor[i].position = { -100, -100 };
        mediumMeteor[i].speed = { 0,0 };
        mediumMeteor[i].radius = 45;
        mediumMeteor[i].active = false;
    }

    for (int i = 0; i < setupSmallmeteor; i++)
    {
        smallMeteor[i].position = { -100, -100 };
        smallMeteor[i].speed = { 0,0 };
        smallMeteor[i].radius = 20;
        smallMeteor[i].active = false;
    }

    midMeteorsCount = 0;
    smallMeteorsCount = 0;

    destroyedMeteorsCount = 0;
}

void DrawMeteors() {

    int framewidth = meteorTexture.width;
    int frameheight = meteorTexture.height;

    Rectangle sourceRec = { 5.0f,5.0f, static_cast<float>(framewidth),static_cast<float>(frameheight) };
    Vector2 Origin = { static_cast<float>(framewidth),static_cast<float>(frameheight) };
    for (int i = 0; i < setupBigmeteor; i++)
    {
        if (isDebug)
        {
            DrawCircle(static_cast<int>(bigMeteor[i].position.x), static_cast<int>(bigMeteor[i].position.y), bigMeteor[i].radius, GREEN);
        }

        Rectangle destRecBig = { bigMeteor[i].position.x, bigMeteor[i].position.y, 250, 200
        };
        if (bigMeteor[i].active) {
            DrawTexturePro(meteorTexture, sourceRec, destRecBig, Origin, bigMeteor[i].position.x + bigMeteor[i].position.y, WHITE);
        }     
    }

    for (int i = 0; i < setupMidmeteor; i++)
    {
        if (isDebug)
        {
            DrawCircle(static_cast<int>(mediumMeteor[i].position.x), static_cast<int>(mediumMeteor[i].position.y), mediumMeteor[i].radius, GREEN);
        }
        Rectangle destRecMid = { mediumMeteor[i].position.x, mediumMeteor[i].position.y, 190, 200 };
        if (mediumMeteor[i].active) DrawTexturePro(meteorTexture, sourceRec, destRecMid, Origin, mediumMeteor[i].position.x + mediumMeteor[i].position.y, WHITE);   
    }

    for (int i = 0; i < setupSmallmeteor; i++)
    {
        if (isDebug)
        {
            DrawCircle(static_cast<int>(smallMeteor[i].position.x), static_cast<int>(smallMeteor[i].position.y), smallMeteor[i].radius, GREEN);
        }
        Vector2 Originsmall = { static_cast<float>(framewidth - 70),static_cast<float>(frameheight - 60) };
        Rectangle destRecSmall = { smallMeteor[i].position.x, smallMeteor[i].position.y, 80, 80 };
        if (smallMeteor[i].active)
            DrawTexturePro(meteorTexture, sourceRec, destRecSmall, Originsmall, smallMeteor[i].position.x + smallMeteor[i].position.y, WHITE);    
    }

    if (destroyedMeteorsCount >= setupBigmeteor + setupMidmeteor + setupSmallmeteor + MaxEnemy) {
        destroyedMeteorsCount = setupBigmeteor + setupMidmeteor + setupSmallmeteor + MaxEnemy;
        Victory(1);
    }

    
}

void LogicMeteor() {

    for (int i = 0; i < setupBigmeteor; i++)
    {
        if (bigMeteor[i].active)
        {

            bigMeteor[i].position.x += bigMeteor[i].speed.x + 40 * GetFrameTime();
            bigMeteor[i].position.y += bigMeteor[i].speed.y + 40 * GetFrameTime();

            if (bigMeteor[i].position.x > GetScreenWidth() + bigMeteor[i].radius) bigMeteor[i].position.x = -(bigMeteor[i].radius);
            else if (bigMeteor[i].position.x < 0 - bigMeteor[i].radius) bigMeteor[i].position.x = GetScreenWidth() + bigMeteor[i].radius;
            if (bigMeteor[i].position.y > GetScreenHeight() + bigMeteor[i].radius) bigMeteor[i].position.y = -(bigMeteor[i].radius);
            else if (bigMeteor[i].position.y < 0 - bigMeteor[i].radius) bigMeteor[i].position.y = GetScreenHeight() + bigMeteor[i].radius;
        }
    }


    for (int i = 0; i < setupMidmeteor; i++)
    {
        if (mediumMeteor[i].active)
        {

            mediumMeteor[i].position.x += mediumMeteor[i].speed.x + 40 * GetFrameTime();
            mediumMeteor[i].position.y += mediumMeteor[i].speed.y + 40 * GetFrameTime();


            if (mediumMeteor[i].position.x > GetScreenWidth() + mediumMeteor[i].radius) mediumMeteor[i].position.x = -(mediumMeteor[i].radius);
            else if (mediumMeteor[i].position.x < 0 - mediumMeteor[i].radius) mediumMeteor[i].position.x = GetScreenWidth() + mediumMeteor[i].radius;
            if (mediumMeteor[i].position.y > GetScreenHeight() + mediumMeteor[i].radius) mediumMeteor[i].position.y = -(mediumMeteor[i].radius);
            else if (mediumMeteor[i].position.y < 0 - mediumMeteor[i].radius) mediumMeteor[i].position.y = GetScreenHeight() + mediumMeteor[i].radius;
        }
    }


    for (int i = 0; i < setupSmallmeteor; i++)
    {
        if (smallMeteor[i].active)
        {

            smallMeteor[i].position.x += smallMeteor[i].speed.x + 40 * GetFrameTime();
            smallMeteor[i].position.y += smallMeteor[i].speed.y + 40 * GetFrameTime();


            if (smallMeteor[i].position.x > GetScreenWidth() + smallMeteor[i].radius) smallMeteor[i].position.x = -(smallMeteor[i].radius);
            else if (smallMeteor[i].position.x < 0 - smallMeteor[i].radius) smallMeteor[i].position.x = GetScreenWidth() + smallMeteor[i].radius;
            if (smallMeteor[i].position.y > GetScreenHeight() + smallMeteor[i].radius) smallMeteor[i].position.y = -(smallMeteor[i].radius);
            else if (smallMeteor[i].position.y < 0 - smallMeteor[i].radius) smallMeteor[i].position.y = GetScreenHeight() + smallMeteor[i].radius;
        }
    }
}

void ColisionMeteors() {
    if (!powerUp2.active && checkPower && powerUp2.lifeSpawn > 0) {
        powerUp2.lifeSpawn++;
    }
    else {

        for (int a = 0; a < setupBigmeteor; a++)
        {
            if (CheckColissionsCircles(player.position.x, player.position.y, bigMeteor[a].position.x, bigMeteor[a].position.y, 19, bigMeteor[a].radius))
            {
                StopMusicStream(background);
                PlaySound(shipCrash);
                DefeatPlayer();
                score = 0;
            }


        }

        for (int a = 0; a < setupMidmeteor; a++)
        {
            if (CheckColissionsCircles(player.position.x, player.position.y, mediumMeteor[a].position.x, mediumMeteor[a].position.y, 19, mediumMeteor[a].radius)) {
                StopMusicStream(background);
                PlaySound(shipCrash);
                DefeatPlayer();
                score = 0;
            }

        }

        for (int a = 0; a < setupSmallmeteor; a++)
        {
            if (CheckColissionsCircles(player.position.x, player.position.y, smallMeteor[a].position.x, smallMeteor[a].position.y, 19, smallMeteor[a].radius)) {
                StopMusicStream(background);
                PlaySound(shipCrash);
                DefeatPlayer();
                score = 0;
            }

        }
    }

    for (int i = 0; i < maxShoots; i++)
    {
        if ((shoot[i].active))
        {
            for (int a = 0; a < setupBigmeteor; a++)
            {
                if (bigMeteor[a].active && CheckColissionsCircles(shoot[i].position.x, shoot[i].position.y, bigMeteor[a].position.x, bigMeteor[a].position.y, shoot[i].radius, bigMeteor[a].radius))
                {
                    score += 30;
                    PlaySoundMulti(meteorImpact);
                    shoot[i].active = false;
                    shoot[i].lifeSpawn = 0;
                    bigMeteor[a].active = false;
                    destroyedMeteorsCount++;

                    for (int j = 0; j < 2; j++)
                    {
                        if (midMeteorsCount % 2 == 0)
                        {
                            mediumMeteor[midMeteorsCount].position = { bigMeteor[a].position.x, bigMeteor[a].position.y };
                            mediumMeteor[midMeteorsCount].speed = { cos(shoot[i].rotation * DEG2RAD) * speedMeteros * -1, sin(shoot[i].rotation * DEG2RAD) * speedMeteros * -1 };
                        }
                        else
                        {
                            mediumMeteor[midMeteorsCount].position = { bigMeteor[a].position.x, bigMeteor[a].position.y };
                            mediumMeteor[midMeteorsCount].speed = { cos(shoot[i].rotation * DEG2RAD) * speedMeteros, sin(shoot[i].rotation * DEG2RAD) * speedMeteros };
                        }

                        mediumMeteor[midMeteorsCount].active = true;
                        midMeteorsCount++;
                    }
                    bigMeteor[a].position = { -100, -100 };
                    a = setupBigmeteor;
                }
            }

            for (int b = 0; b < setupMidmeteor; b++)
            {
                if (mediumMeteor[b].active && CheckColissionsCircles(shoot[i].position.x, shoot[i].position.y, mediumMeteor[b].position.x, mediumMeteor[b].position.y, shoot[i].radius, mediumMeteor[b].radius))
                {
                    score += 20;
                    PlaySoundMulti(meteorImpact);
                    shoot[i].active = false;
                    shoot[i].lifeSpawn = 0;
                    mediumMeteor[b].active = false;
                    destroyedMeteorsCount++;

                    for (int j = 0; j < 2; j++)
                    {
                        if (smallMeteorsCount % 2 == 0)
                        {
                            smallMeteor[smallMeteorsCount].position = { mediumMeteor[b].position.x, mediumMeteor[b].position.y };
                            smallMeteor[smallMeteorsCount].speed = { cos(shoot[i].rotation * DEG2RAD) * speedMeteros * -1, sin(shoot[i].rotation * DEG2RAD) * speedMeteros * -1 };
                        }
                        else
                        {
                            smallMeteor[smallMeteorsCount].position = { mediumMeteor[b].position.x, mediumMeteor[b].position.y };
                            smallMeteor[smallMeteorsCount].speed = { cos(shoot[i].rotation * DEG2RAD) * speedMeteros, sin(shoot[i].rotation * DEG2RAD) * speedMeteros };
                        }

                        smallMeteor[smallMeteorsCount].active = true;
                        smallMeteorsCount++;
                    }
                    mediumMeteor[b].position = { -100, -100 };
                    b = setupMidmeteor;
                }
            }

            for (int c = 0; c < setupSmallmeteor; c++)
            {
                if (smallMeteor[c].active && CheckColissionsCircles(shoot[i].position.x, shoot[i].position.y, smallMeteor[c].position.x, smallMeteor[c].position.y, shoot[i].radius, smallMeteor[c].radius))
                {
                    score += 10;
                    PlaySoundMulti(meteorImpact);
                    shoot[i].active = false;
                    shoot[i].lifeSpawn = 0;
                    smallMeteor[c].active = false;
                    destroyedMeteorsCount++;
                    smallMeteor[c].position = { -100, -100 };
                    c = setupSmallmeteor;
                }
            }
        }
    }
}


//LOGIC POWER UPS
void PowerUpsSetup() {

    float posx;
    float posy;
    float posx2;
    float posy2;
    bool correctRange = false;

    powerUp.active = false;
    powerUp2.active = false;

    posx = static_cast<float>(GetRandomValue(0, GetScreenWidth()));
    posx2 = static_cast<float>(GetRandomValue(0, GetScreenWidth()));

    while (!correctRange)
    {
        if (posx > GetScreenWidth() / 2 && posx < GetScreenWidth() / 2) posx = static_cast<float>(GetRandomValue(0, GetScreenWidth()) - 100);
        else correctRange = true;
        if (posx2 > GetScreenWidth() / 2 && posx2 < GetScreenWidth() / 2) posx2 = static_cast<float>(GetRandomValue(0, GetScreenWidth()) - 100);
        else correctRange = true;
    }

    correctRange = false;

    posy = static_cast<float>(GetRandomValue(0, GetScreenHeight()));
    posy2 = static_cast<float>(GetRandomValue(0, GetScreenHeight()));

    while (!correctRange)
    {
        if (posy2 > GetScreenHeight() / 2 && posy2 < GetScreenHeight() / 2)  posy2 = static_cast<float>(GetRandomValue(0, GetScreenHeight()) - 100);
        else correctRange = true;
        if (posy2 > GetScreenHeight() / 2 && posy2 < GetScreenHeight() / 2)  posy2 = static_cast<float>(GetRandomValue(0, GetScreenHeight()) - 100);
        else correctRange = true;
    }

    powerUp.position = { posx, posy };
    powerUp2.position = { posx2, posy2 };

    powerUp.radius = 35;
    powerUp.speed = { 0,0 };
    powerUp.rotation = 0;
    powerUp.lifeSpawn = 0;

    powerUp2.radius = 35;
    powerUp2.speed = { 0,0 };
    powerUp2.rotation = 0;
    powerUp2.lifeSpawn = 0;

    checkPower = false;

}

void PowerUpDraw() {

    if (powerUp.active)
    {
        DrawCircle(static_cast<int>(powerUp.position.x), static_cast<int>(powerUp.position.y), powerUp.radius, GREEN);
        DrawTextureEx(shootexture, { powerUp.position.x - 197,powerUp.position.y - 197 }, 0, 4, GREEN);
    }

    if (powerUp2.active)
    {
        DrawCircle(static_cast<int>(powerUp2.position.x), static_cast<int>(powerUp2.position.y), powerUp2.radius, YELLOW);
        DrawTextureEx(ship, { powerUp2.position.x + 97,powerUp2.position.y - 105 }, 90, 2, YELLOW);
    }
}

void PowerUpLogic() {
    int random = 0;
    int random2 = 0;

    if (powerUp.active)
    {
        if (CheckColissionsCircles(powerUp.position.x, powerUp.position.y, player.position.x, player.position.y, powerUp.radius, 19))
        {

            checkPower = true;
            powerUp.active = false;
            powerUp.lifeSpawn++;

        }
    }

    if (powerUp.lifeSpawn > 500) {
        powerUp.lifeSpawn = 0;
        if (powerUp2.lifeSpawn == 0)
        {
            PowerUpsSetup();
        }
    }

    if (!powerUp.active && powerUp.lifeSpawn == 0)
    {
        random = GetRandomValue(0, 500);
        if (random == 50)
        {
            powerUp.active = true;
        }
    }

    if (powerUp2.active)
    {
        if (CheckColissionsCircles(powerUp2.position.x, powerUp2.position.y, player.position.x, player.position.y, powerUp2.radius, 19))
        {

            checkPower = true;
            powerUp2.active = false;
            powerUp2.lifeSpawn++;

        }
    }

    if (powerUp2.lifeSpawn > 500) {
        powerUp2.lifeSpawn = 0;
        if (powerUp.lifeSpawn == 0)
        {
            PowerUpsSetup();
        }

    }

    if (!powerUp2.active && powerUp2.lifeSpawn == 0)
    {
        random2 = GetRandomValue(0, 500);
        if (random2 == 50)
        {
            powerUp2.active = true;
        }
    }
}

//LOGIC ENEMY
void EnemySetup() {
    for (int i = 0; i < MaxEnemy; i++)
    {
        enemy[i].position = { static_cast<float>(GetScreenWidth()) / 2, static_cast<float>(GetScreenHeight()) / 2 };
        enemy[i].speed = { 0, 0 };
        enemy[i].rotation = 0;
        enemy[i].active = true;

        float posx;
        float posy;
        bool correctRange = false;

        posx = static_cast<float>(GetRandomValue(0, GetScreenWidth()));

        while (!correctRange)
        {
            if (posx > GetScreenWidth() / 2 - 150 && posx < GetScreenWidth() / 2 + 150) posx = static_cast<float>(GetRandomValue(0, GetScreenWidth()));
            else correctRange = true;
        }

        correctRange = false;

        posy = static_cast<float>(GetRandomValue(0, GetScreenHeight()));

        while (!correctRange)
        {
            if (posy > GetScreenHeight() / 2 - 150 && posy < GetScreenHeight() / 2 + 150)  posy = static_cast<float>(GetRandomValue(0, GetScreenHeight()));
            else correctRange = true;
        }

        enemy[i].position = { posx, posy };
    }
}

void LogicEnemy() {
    for (int i = 0; i < MaxEnemy; i++)
    {

        enemy[i].speed.x = sin(enemy[i].rotation * DEG2RAD) * enemySpeed;
        enemy[i].speed.y = cos(enemy[i].rotation * DEG2RAD) * enemySpeed;

        enemy[i].position.x += (enemy[i].speed.x * 5) * GetFrameTime();
        enemy[i].position.y -= (enemy[i].speed.y * 5) * GetFrameTime();

        if (enemy[i].position.x > GetScreenWidth() + shipHeight) {
            enemy[i].active = false;
        }
        else if (enemy[i].position.x < -(shipHeight)) {
            enemy[i].active = false;
        }

        if (enemy[i].position.y > (GetScreenHeight() + shipHeight)) {
            enemy[i].active = false;
        }
        else if (enemy[i].position.y < -(shipHeight)) {
            enemy[i].active = false;
        }
    }
}

void DrawEnemy() {
    int framewidth = ship.width;
    int frameheight = ship.height;

    for (int i = 0; i < MaxEnemy; i++)
    {
        float angulosradianes = atan2(enemy[i].position.y - player.position.y, enemy[i].position.x - player.position.x);
        float angulogrados = (180 / PI) * angulosradianes - 90;


        Rectangle sourceRec = { 5.0f,5.0f, static_cast<float>(framewidth),static_cast<float>(frameheight) };
        Rectangle destRec = { enemy[i].position.x, enemy[i].position.y, 200, 200 };
        Vector2 Origin = { static_cast<float>(framewidth),static_cast<float>(frameheight) };

        

        if (enemy[i].active)
        {
            if (CheckColissionsCircles(enemy[i].position.x, enemy[i].position.y, player.position.x, player.position.y, 25, 19))
            {
                StopMusicStream(background);
                PlaySound(shipCrash);
                DefeatPlayer();
                score = 0;
            }
            if (destroyedMeteorsCount == setupBigmeteor + setupMidmeteor + setupSmallmeteor) {
                angulogrados = (180 / PI) * angulosradianes - 270;
                DrawTexturePro(ship, sourceRec, destRec, Origin, enemy[i].rotation, BLUE);
            }
            else
            {
                DrawTexturePro(ship, sourceRec, destRec, Origin, enemy[i].rotation, RED);
            }

            enemy[i].rotation = angulogrados;

        }
        if (enemy[i].active == false)
        {
            destroyedMeteorsCount = setupBigmeteor + setupMidmeteor + setupSmallmeteor + MaxEnemy;
        }

        if (isDebug && enemy[i].active)
        {
            DrawCircle(static_cast<int>(enemy[i].position.x), static_cast<int>(enemy[i].position.y), 25, BROWN);
        }
    }

}


//SCORE
void DrawScoreMenu() {
    ifstream readMyFile("rsc/score.txt");
    if (maxScore == 0)
    {
        readMyFile >> maxScore;
    }    
    readMyFile.close();

    ofstream creatMyFile("rsc/score.txt");
    creatMyFile << maxScore;
    
    
    if (maxScore > 0)
    {    
        DrawText(TextFormat("Max. Puntuacion: %i ", maxScore), GetScreenWidth() / 2 - 300, 10, 60, RED);
    }
    creatMyFile.close();
   
}

void DrawScore() {
    DrawText(TextFormat("Max. Puntuacion: %i ", maxScore), GetScreenWidth() - 350, 10, 30, RED);
    DrawText(TextFormat("Puntuacion: %i ", score), 10, 10, 20, WHITE);
    DrawText(TextFormat("/ %i ", setupBigmeteor + setupMidmeteor + setupSmallmeteor + MaxEnemy ), GetScreenWidth() - 430, 80, 30, WHITE);
    DrawText(TextFormat("Contador: %i ", destroyedMeteorsCount), GetScreenWidth() - 630, 80, 30, WHITE);
    DrawText(TextFormat("Nivel: %i ", level), GetScreenWidth() - 750, 10, 20, WHITE);
    if (maxScore < score)
    {
        maxScore = score;
    }
    if (maxScore == score && maxScore != 0)
    {
        DrawText("NUEVO RECORD", GetScreenWidth() - 630, 10, 30, RED);
    }
}


//Mode Debug

void CheckDebug(bool check) {
    if (check)
    {
        isDebug = true;
    }
    else
    {
        isDebug = false;
    }

}
