
#include "raylib.h"
#include <cmath>
#include "player.h"

void PlayerDraw() {

    Vector2 v1 = { player.position.x + sinf(player.rotation * DEG2RAD) * (shipHeight), player.position.y - cosf(player.rotation * DEG2RAD) * (shipHeight) };
    Vector2 v2 = { player.position.x - cosf(player.rotation * DEG2RAD) * (playerBaseSize / 2), player.position.y - sinf(player.rotation * DEG2RAD) * (playerBaseSize / 2) };
    Vector2 v3 = { player.position.x + cosf(player.rotation * DEG2RAD) * (playerBaseSize / 2), player.position.y + sinf(player.rotation * DEG2RAD) * (playerBaseSize / 2) };
    DrawTriangle(v1, v2, v3, MAROON);
}

void SetupPlayer() {

    shipHeight = (playerBaseSize / 2) / tanf(20 * DEG2RAD);

    player.position = { (float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2  };
    player.speed = { 0, 0 };
    player.acceleration = 40;
    player.rotation = 0;
    player.collider = { player.position.x + sin(player.rotation * DEG2RAD) * (shipHeight / 2.5f), player.position.y - cos(player.rotation * DEG2RAD) * (shipHeight / 2.5f), 12 };
    player.color = LIGHTGRAY;
}

void MovePlayer() {

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

void ColisionWall() {

    if (player.position.x > GetScreenWidth() + shipHeight) player.position.x = -(shipHeight);
    else if (player.position.x < -(shipHeight)) player.position.x = GetScreenWidth() + shipHeight;
    if (player.position.y > (GetScreenHeight() + shipHeight)) player.position.y = -(shipHeight);
    else if (player.position.y < -(shipHeight)) player.position.y = GetScreenHeight() + shipHeight;
}