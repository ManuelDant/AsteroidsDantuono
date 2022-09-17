#pragma once
#include "raylib.h"

typedef struct Player {
    Vector2 position;
    Vector2 speed;
    float acceleration;
    float rotation;
    Vector2 Rotation;
    Vector3 collider;
    Color color;
}Player;

static Player player = { 0 };

static float playerBaseSize = 20.0f;
static float playerSpeed = 6.0f;
static float shipHeight = 0.0f;

void PlayerDraw();
void SetupPlayer();
void MovePlayer();
void ColisionWall();