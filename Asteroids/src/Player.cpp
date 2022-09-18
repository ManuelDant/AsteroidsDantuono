#include "raylib.h"
#include <cmath>
#include "player.h"

void PlayerDraw() {

    Vector2 v1 = { player.position.x + sinf(player.rotation * DEG2RAD) * (shipHeight), player.position.y - cosf(player.rotation * DEG2RAD) * (shipHeight) };
    Vector2 v2 = { player.position.x - cosf(player.rotation * DEG2RAD) * (playerBaseSize / 2), player.position.y - sinf(player.rotation * DEG2RAD) * (playerBaseSize / 2) };
    Vector2 v3 = { player.position.x + cosf(player.rotation * DEG2RAD) * (playerBaseSize / 2), player.position.y + sinf(player.rotation * DEG2RAD) * (playerBaseSize / 2) };
    DrawTriangle(v1, v2, v3, MAROON);

    for (int i = 0; i < maxShoots; i++)
    {
        if (shoot[i].active) DrawCircleV(shoot[i].position, shoot[i].radius, BLACK);
    }
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
        shoot[i].color = RED;
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

    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
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