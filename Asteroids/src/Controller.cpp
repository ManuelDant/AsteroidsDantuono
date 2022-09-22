#include "player.h"
#include "meteor.h"
#include "game.h"
#include "assetsGame.h"
#include <cmath>

void LoadResourcesGame() {
    meteorTexture = LoadTexture("rsc/meteor.png");
    ship = LoadTexture("rsc/ship.png");
    shootexture = LoadTexture("rsc/shoot.png");
    mira = LoadTexture("rsc/mira.png");
    bgroundgame = LoadTexture("rsc/bgroundgame.png");  
    shipShoot = LoadSound("shoot.mp3");
    meteorImpact = LoadSound("meteorImpact.mp3");
    shipCrash = LoadSound("shipCrash.mp3");
    background = LoadMusicStream("music.mp3");  
    SetSoundVolume(meteorImpact, 0.3f);    
    SetSoundVolume(shipCrash, 0.5f);
    SetSoundVolume(shipShoot, 2);
}

void UnloadResourcesGame() {
    UnloadSound(shipShoot);
    UnloadSound(meteorImpact);
    UnloadSound(shipCrash);
    UnloadMusicStream(background);
    UnloadTexture(ship);
    UnloadTexture(shootexture);
    UnloadTexture(meteorTexture);
    UnloadTexture(mira);
    UnloadTexture(bgroundgame);  
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

void EnemySetup() {
    for (int i = 0; i < MaxEnemy; i++)
    {
        enemy[i].position = { (float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2 };
        enemy[i].speed = { 0, 0 };
        enemy[i].acceleration = 40;
        enemy[i].rotation = 0;
        enemy[i].active = true;

        float posx;
        float posy;
        bool correctRange = false;

        posx = GetRandomValue(0, GetScreenWidth());

        while (!correctRange)
        {
            if (posx > GetScreenWidth() / 2 - 150 && posx < GetScreenWidth() / 2 + 150) posx = GetRandomValue(0, GetScreenWidth());
            else correctRange = true;
        }

        correctRange = false;

        posy = GetRandomValue(0, GetScreenHeight());

        while (!correctRange)
        {
            if (posy > GetScreenHeight() / 2 - 150 && posy < GetScreenHeight() / 2 + 150)  posy = GetRandomValue(0, GetScreenHeight());
            else correctRange = true;
        }

        enemy[i].position = { posx, posy };
    }
}

void BackgroundGame() {
        scrollingBack -= 5.0f;
  
    if (scrollingBack <= -bgroundgame.width * 2 ) scrollingBack = 0;
    DrawTextureEx(bgroundgame, { scrollingBack }, 0.0f, 2.0f, WHITE);
    DrawTextureEx(bgroundgame, { bgroundgame.width * 2 + scrollingBack }, 0.0f, 2.0f, WHITE);
}

void LogicEnemy() {
    for (int i = 0; i < MaxEnemy; i++)
    {

        enemy[i].speed.x = sin(enemy[i].rotation * DEG2RAD) * playerSpeed;
        enemy[i].speed.y = cos(enemy[i].rotation * DEG2RAD) * playerSpeed;

        enemy[i].position.x += (enemy[i].speed.x * player.acceleration / 4) * GetFrameTime();
        enemy[i].position.y -= (enemy[i].speed.y * player.acceleration / 4) * GetFrameTime();

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

            
        Rectangle sourceRec = { 5.0f,5.0f, (float)framewidth,(float)frameheight };
        Rectangle destRec = { enemy[i].position.x, enemy[i].position.y, 200, 200 };
        Vector2 Origin = { (float)framewidth,(float)frameheight };

        if (enemy[i].active)
        {
            if (CheckColissionsCircles(enemy[i].position.x, enemy[i].position.y, player.position.x, player.position.y, 25, 19))
            {
                StopMusicStream(background);
                PlaySound(shipCrash);
                DefeatPlayer();
            }         
            if (destroyedMeteorsCount == maxBigMeteors + maxMidMeteors + maxSmallMeteors) {
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
            destroyedMeteorsCount++;
        }
    }
}

void PlayerDraw() {
    int framewidth = ship.width;
    int frameheight = ship.height;

    BackgroundGame();

    Rectangle sourceRec = { 5.0f,5.0f, (float)framewidth,(float)frameheight };
    Rectangle destRec = { player.position.x, player.position.y, 200, 200};
    Vector2 Origin = { (float)framewidth,(float)frameheight };

    DrawTexturePro(ship, sourceRec, destRec, Origin, player.rotation, WHITE);

    PlayMusicStream(background);
    UpdateMusicStream(background);

    for (int i = 0; i < maxShoots; i++)
    {
        if (shoot[i].active) { 
            DrawTexture(shootexture, shoot[i].position.x - 50, shoot[i].position.y - 50, WHITE); }      
    }

    HideCursor();
    DrawTexturePro(mira, { 5.0f,5.0f, (float)mira.width,(float)mira.height }, { (float)GetMouseX() - 17,(float)GetMouseY() - 5, 250,250 }, { (float)mira.width,(float)mira.height}, 0, WHITE);

}

void SetupPlayer() {
    StopMusicStream(background);
    shipHeight = (playerBaseSize / 2) / tanf(20 * DEG2RAD);

    player.position = { (float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2};
    player.speed = { 0, 0 };
    player.acceleration = 40;
    player.rotation = 0;
    player.color = LIGHTGRAY;
    

    for (int i = 0; i < maxShoots; i++)
    {
        shoot[i].position = { (float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2 };
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

void LogicShoot() {
    
    if (IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        PlaySoundMulti(shipShoot);
        for (int i = 0; i < maxShoots; i++)
        {
            if (!shoot[i].active)
            {
                shoot[i].position = { player.position.x + sin(player.rotation * DEG2RAD) * (shipHeight), player.position.y - cos(player.rotation * DEG2RAD) * (shipHeight) };
                shoot[i].active = true;
                shoot[i].speed.x = 1.5 * sin(player.rotation * DEG2RAD) * playerSpeed;
                shoot[i].speed.y = 1.5 * cos(player.rotation * DEG2RAD) * playerSpeed;
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
            if (destroyedMeteorsCount == maxBigMeteors + maxMidMeteors + maxSmallMeteors) {
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

void LogicPlayer() {

    float angulosradianes = atan2(GetMouseY() - player.position.y, GetMouseX() - player.position.x);
    float angulogrados = (180 / PI) * angulosradianes - 270;


    player.rotation = angulogrados;

    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
    {
        player.speed.x = sin(player.rotation * DEG2RAD) * playerSpeed;
        player.speed.y = cos(player.rotation * DEG2RAD) * playerSpeed;

        if (player.acceleration < 1) player.acceleration += 0.01f;

    }
    else
    {
        if (player.acceleration > 0) player.acceleration = player.acceleration;
        else if (player.acceleration < 0) player.acceleration = 0;
    }

    player.position.x += (player.speed.x * player.acceleration) * GetFrameTime();
    player.position.y -= (player.speed.y * player.acceleration) * GetFrameTime();
}

//LOGIC ASTEROIDS/PLAYER
void SetupMeteor() {
    float posx;
    float posy;
    float velx;
    float vely;
    bool correctRange = false;

    for (int i = 0; i < maxBigMeteors; i++)
    {
        posx = GetRandomValue(0, GetScreenWidth());

        while (!correctRange)
        {
            if (posx > GetScreenWidth() / 2 - 150 && posx < GetScreenWidth() / 2 + 150) posx = GetRandomValue(0, GetScreenWidth());
            else correctRange = true;
        }

        correctRange = false;

        posy = GetRandomValue(0, GetScreenHeight());

        while (!correctRange)
        {
            if (posy > GetScreenHeight() / 2 - 150 && posy < GetScreenHeight() / 2 + 150)  posy = GetRandomValue(0, GetScreenHeight());
            else correctRange = true;
        }

        bigMeteor[i].position = { posx, posy };

        correctRange = false;
        velx = GetRandomValue(-speedMeteros, speedMeteros);
        vely = GetRandomValue(-speedMeteros, speedMeteros);

        while (!correctRange)
        {
            if (velx == 0 && vely == 0)
            {
                velx = GetRandomValue(-speedMeteros, speedMeteros);
                vely = GetRandomValue(-speedMeteros, speedMeteros);
            }
            else correctRange = true;
        }

        bigMeteor[i].speed = { velx, vely };
        bigMeteor[i].radius = 55;
        bigMeteor[i].active = true;
    }

    for (int i = 0; i < maxMidMeteors; i++)
    {
        mediumMeteor[i].position = { -100, -100 };
        mediumMeteor[i].speed = { 0,0 };
        mediumMeteor[i].radius = 45;
        mediumMeteor[i].active = false;
    }

    for (int i = 0; i < maxSmallMeteors; i++)
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

    Rectangle sourceRec = { 5.0f,5.0f, (float)framewidth,(float)frameheight };
    Vector2 Origin = { (float)framewidth,(float)frameheight };
    for (int i = 0; i < maxBigMeteors; i++)
    {   
        Rectangle destRecBig = { bigMeteor[i].position.x, bigMeteor[i].position.y, 250, 200
    };
        if (bigMeteor[i].active) {
            DrawTexturePro(meteorTexture, sourceRec, destRecBig, Origin, bigMeteor[i].position.x + bigMeteor[i].position.y, WHITE);
        }    
    }

    for (int i = 0; i < maxMidMeteors; i++)
    {
        Rectangle destRecMid = { mediumMeteor[i].position.x, mediumMeteor[i].position.y, 190, 200 };
        if (mediumMeteor[i].active) DrawTexturePro(meteorTexture, sourceRec, destRecMid, Origin, mediumMeteor[i].position.x + mediumMeteor[i].position.y, WHITE);
       
    }

    for (int i = 0; i < maxSmallMeteors; i++)
    {
        Vector2 Originsmall = { (float)framewidth - 70,(float)frameheight - 60 };
        Rectangle destRecSmall = { smallMeteor[i].position.x, smallMeteor[i].position.y, 80, 80 };
        if (smallMeteor[i].active)
            DrawTexturePro(meteorTexture, sourceRec, destRecSmall, Originsmall, smallMeteor[i].position.x + smallMeteor[i].position.y, WHITE);
        
    }

    if (destroyedMeteorsCount >= maxBigMeteors + maxMidMeteors + maxSmallMeteors + MaxEnemy) { 
        destroyedMeteorsCount = maxBigMeteors + maxMidMeteors + maxSmallMeteors + MaxEnemy;
        Victory(1); }

}

void LogicMeteor() {

    for (int i = 0; i < maxBigMeteors; i++)
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


    for (int i = 0; i < maxMidMeteors; i++)
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


    for (int i = 0; i < maxSmallMeteors; i++)
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

    for (int a = 0; a < maxBigMeteors; a++)
    {
        if (CheckColissionsCircles(player.position.x,player.position.y,bigMeteor[a].position.x, bigMeteor[a].position.y, 19, bigMeteor[a].radius)) 
        {
            StopMusicStream(background);
            PlaySound(shipCrash);
            DefeatPlayer();
        }
        
    }

    for (int a = 0; a < maxMidMeteors; a++)
    {
        if (CheckColissionsCircles(player.position.x, player.position.y, mediumMeteor[a].position.x, mediumMeteor[a].position.y, 19, mediumMeteor[a].radius)) {
            StopMusicStream(background);
            PlaySound(shipCrash);
            DefeatPlayer(); }
    }

    for (int a = 0; a < maxSmallMeteors; a++)
    {
        if (CheckColissionsCircles(player.position.x, player.position.y, smallMeteor[a].position.x, smallMeteor[a].position.y, 19, smallMeteor[a].radius)) {
            StopMusicStream(background);
            PlaySound(shipCrash);
            DefeatPlayer(); }
    }

    for (int i = 0; i < maxShoots; i++)
    {
        if ((shoot[i].active))
        {
            for (int a = 0; a < maxBigMeteors; a++)
            {
                if (bigMeteor[a].active && CheckColissionsCircles(shoot[i].position.x, shoot[i].position.y, bigMeteor[a].position.x, bigMeteor[a].position.y, shoot[i].radius, bigMeteor[a].radius))
                {
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
                    a = maxBigMeteors;
                }
            }

            for (int b = 0; b < maxMidMeteors; b++)
            {
                if (mediumMeteor[b].active && CheckColissionsCircles(shoot[i].position.x, shoot[i].position.y, mediumMeteor[b].position.x, mediumMeteor[b].position.y, shoot[i].radius, mediumMeteor[b].radius))
                {
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
                    b = maxMidMeteors;
                }
            }

            for (int c = 0; c < maxSmallMeteors; c++)
            {
                if (smallMeteor[c].active && CheckColissionsCircles(shoot[i].position.x, shoot[i].position.y, smallMeteor[c].position.x, smallMeteor[c].position.y, shoot[i].radius, smallMeteor[c].radius))
                {
                    PlaySoundMulti(meteorImpact);
                    shoot[i].active = false;
                    shoot[i].lifeSpawn = 0;
                    smallMeteor[c].active = false;
                    destroyedMeteorsCount++;
                    smallMeteor[c].position = { -100, -100 };
                    c = maxSmallMeteors;
                }
            }
        }
    }
}