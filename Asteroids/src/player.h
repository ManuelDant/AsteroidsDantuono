#pragma once
#include "raylib.h"

struct Player {
    Vector2 position;
    Vector2 speed;
    float acceleration;
    float rotation;
    Color color;
    bool active;
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

const int maxShoots = 15;
static bool hitbox = false;

static Player player;

static const int MaxEnemy = 1;
static Player enemy[MaxEnemy];

static Shoot shoot[maxShoots];

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
void EnemySetup();
void DrawEnemy();
void LogicEnemy();
bool CheckColissionsCircles(float c1x, float c1y, float c2x, float c2y, float c1r, float c2r);
void DrawScore();

