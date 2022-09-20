#pragma once
#include "raylib.h"

struct Player {
    Vector2 position;
    Vector2 speed;
    float acceleration;
    float rotation;
    Vector2 Rotation;
    Vector3 collider;
    Color color;
};

struct Shoot {
    Vector2 position;
    Vector2 speed;
    float radius;
    float rotation;
    int lifeSpawn;
    bool active;
    Color color;
};

static Texture2D ship;
static Texture2D shootexture;

static Sound shipShoot;
static Sound shipCrash;
static Music background;

const int maxShoots = 10;
static bool hitbox = false;

static Player player = { 0 };
static Shoot shoot[maxShoots] = { 0 };

static float playerBaseSize = 20.0f;
static float playerSpeed = 6.0f;
static float shipHeight = 0.0f;

void PlayerDraw();
void SetupPlayer();
void MovePlayer();
void ColisionWall();
void LogicShoot();
void LogicPlayer();
void DefeatPlayer();
void Restart();
void Victory(bool victory);
void LoadResources();
void UnloadResources();


