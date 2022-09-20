#pragma once
#include "raylib.h"

struct Meteor {
    Vector2 position;
    Vector2 speed;
    float radius;
    bool active;
    Color color;
};

static Texture2D meteorTexture;

static Sound meteorImpact;

const int speedMeteros =2;
const int maxBigMeteors  =5;
const int maxMidMeteors =10;
const int maxSmallMeteors =20;

static Meteor bigMeteor[maxBigMeteors] = { 0 };
static Meteor mediumMeteor[maxMidMeteors] = { 0 };
static Meteor smallMeteor[maxSmallMeteors] = { 0 };

static int midMeteorsCount = 0;
static int smallMeteorsCount = 0;
static int destroyedMeteorsCount = 0;

void ColisionMeteors();
void LogicMeteor();
void SetupMeteor();
void DrawMeteors();