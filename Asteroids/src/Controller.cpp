#include "raylib.h"
#include "player.h"
#include "meteor.h"
#include "game.h"
#include <cmath>

void PlayerDraw() {
    Texture2D ship = LoadTexture("src/ship.png");
    int framewidth = ship.width;
    int frameheight = ship.height;

    Rectangle sourceRec = { 5.0f,5.0f, (float)framewidth,(float)frameheight };
    Rectangle destRec = { player.position.x, player.position.y, 200, 200};
    Vector2 Origin = { (float)framewidth,(float)frameheight };

    Vector2 v1 = { player.position.x + sinf(player.rotation * DEG2RAD) * (shipHeight), player.position.y - cosf(player.rotation * DEG2RAD) * (shipHeight) };
    Vector2 v2 = { player.position.x - cosf(player.rotation * DEG2RAD) * (playerBaseSize / 2), player.position.y - sinf(player.rotation * DEG2RAD) * (playerBaseSize / 2) };
    Vector2 v3 = { player.position.x + cosf(player.rotation * DEG2RAD) * (playerBaseSize / 2), player.position.y + sinf(player.rotation * DEG2RAD) * (playerBaseSize / 2) };
    DrawTexturePro(ship, sourceRec, destRec, Origin, player.rotation, WHITE);

    for (int i = 0; i < maxShoots; i++)
    {
        if (shoot[i].active) DrawCircleV(shoot[i].position, shoot[i].radius, WHITE);
    }
    UnloadTexture(texture);
}

void SetupPlayer() {

    shipHeight = (playerBaseSize / 2) / tanf(20 * DEG2RAD);

    player.position = { (float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2};
    player.speed = { 0, 0 };
    player.acceleration = 40;
    player.rotation = 0;
    player.collider = { player.position.x + sin(player.rotation * DEG2RAD) * (shipHeight / 2.5f), player.position.y - cos(player.rotation * DEG2RAD) * (shipHeight / 2.5f), 12 };
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

    // Shot logic
    for (int i = 0; i < maxShoots; i++)
    {
        if (shoot[i].active)
        {
            // Movement
            shoot[i].position.x += shoot[i].speed.x;
            shoot[i].position.y -= shoot[i].speed.y;

            // Collision logic: shoot vs walls
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

            // Life of shoot
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

        if (player.acceleration < 1) player.acceleration += 0.04f;

    }
    else
    {
        if (player.acceleration > 0) player.acceleration = player.acceleration;
        else if (player.acceleration < 0) player.acceleration = 0;
    }
    if (IsKeyDown(KEY_DOWN))
    {
        if (player.acceleration > 0) player.acceleration -= 0.04f;
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
        bigMeteor[i].color = BLUE;
    }

    for (int i = 0; i < maxMidMeteors; i++)
    {
        mediumMeteor[i].position = { -100, -100 };
        mediumMeteor[i].speed = { 0,0 };
        mediumMeteor[i].radius = 45;
        mediumMeteor[i].active = false;
        mediumMeteor[i].color = BLUE;
    }

    for (int i = 0; i < maxSmallMeteors; i++)
    {
        smallMeteor[i].position = { -100, -100 };
        smallMeteor[i].speed = { 0,0 };
        smallMeteor[i].radius = 20;
        smallMeteor[i].active = false;
        smallMeteor[i].color = BLUE;
    }

    midMeteorsCount = 0;
    smallMeteorsCount = 0;
}

void DrawMeteors() {
    Texture2D texture = LoadTexture("src/meteor.png");
    int framewidth = texture.width;
    int frameheight = texture.height;

    Rectangle sourceRec = { 5.0f,5.0f, (float)framewidth,(float)frameheight };
    Vector2 Origin = { (float)framewidth,(float)frameheight };
    for (int i = 0; i < maxBigMeteors; i++)
    {   
        Rectangle destRecBig = { bigMeteor[i].position.x, bigMeteor[i].position.y, 250, 200
    };
        if (bigMeteor[i].active) {
            DrawTexturePro(texture, sourceRec, destRecBig, Origin, bigMeteor[i].position.x + bigMeteor[i].position.y, WHITE);         
        }    
    }

    for (int i = 0; i < maxMidMeteors; i++)
    {
        Rectangle destRecMid = { mediumMeteor[i].position.x, mediumMeteor[i].position.y, 190, 200 };
        if (mediumMeteor[i].active) DrawTexturePro(texture, sourceRec, destRecMid, Origin, mediumMeteor[i].position.x + mediumMeteor[i].position.y, WHITE); 
       
    }

    for (int i = 0; i < maxSmallMeteors; i++)
    {
        Vector2 Originsmall = { (float)framewidth - 70,(float)frameheight - 60 };
        Rectangle destRecSmall = { smallMeteor[i].position.x, smallMeteor[i].position.y, 80, 80 };
        if (smallMeteor[i].active)
            DrawTexturePro(texture, sourceRec, destRecSmall, Originsmall, smallMeteor[i].position.x + smallMeteor[i].position.y, WHITE);
        
    }
    if (destroyedMeteorsCount == maxBigMeteors + maxMidMeteors + maxSmallMeteors) DefeatPlayer();

}

void LogicMeteor() {

    for (int i = 0; i < maxBigMeteors; i++)
    {
        if (bigMeteor[i].active)
        {

            bigMeteor[i].position.x += bigMeteor[i].speed.x;
            bigMeteor[i].position.y += bigMeteor[i].speed.y;

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

            mediumMeteor[i].position.x += mediumMeteor[i].speed.x;
            mediumMeteor[i].position.y += mediumMeteor[i].speed.y;


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

            smallMeteor[i].position.x += smallMeteor[i].speed.x;
            smallMeteor[i].position.y += smallMeteor[i].speed.y;


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
        if (CheckCollisionCircles({ player.position.x , player.position.y }, 19, bigMeteor[a].position, bigMeteor[a].radius) && bigMeteor[a].active) {
            DefeatPlayer();
        }
        
    }

    for (int a = 0; a < maxMidMeteors; a++)
    {
        if (CheckCollisionCircles({ player.position.x , player.position.y }, 19, mediumMeteor[a].position, mediumMeteor[a].radius) && mediumMeteor[a].active) DefeatPlayer();
    }

    for (int a = 0; a < maxSmallMeteors; a++)
    {
        if (CheckCollisionCircles({ player.position.x , player.position.y }, 19, smallMeteor[a].position, smallMeteor[a].radius) && smallMeteor[a].active) DefeatPlayer();
    }

    for (int i = 0; i < maxShoots; i++)
    {
        if ((shoot[i].active))
        {
            for (int a = 0; a < maxBigMeteors; a++)
            {
                if (bigMeteor[a].active && CheckCollisionCircles(shoot[i].position, shoot[i].radius, bigMeteor[a].position, bigMeteor[a].radius))
                {
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
                    bigMeteor[a].color = RED;
                    a = maxBigMeteors;
                }
            }

            for (int b = 0; b < maxMidMeteors; b++)
            {
                if (mediumMeteor[b].active && CheckCollisionCircles(shoot[i].position, shoot[i].radius, mediumMeteor[b].position, mediumMeteor[b].radius))
                {
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
                    mediumMeteor[b].color = GREEN;
                    b = maxMidMeteors;
                }
            }

            for (int c = 0; c < maxSmallMeteors; c++)
            {
                if (smallMeteor[c].active && CheckCollisionCircles(shoot[i].position, shoot[i].radius, smallMeteor[c].position, smallMeteor[c].radius))
                {
                    shoot[i].active = false;
                    shoot[i].lifeSpawn = 0;
                    smallMeteor[c].active = false;
                    destroyedMeteorsCount++;
                    smallMeteor[c].color = YELLOW;
                    smallMeteor[c].position = { -100, -100 };
                    c = maxSmallMeteors;
                }
            }
        }
    }
}