#pragma once
#include "raylib.h"

struct Meteor {
    Vector2 position;
    Vector2 speed;
    float radius;
    bool active;
};

static Texture2D meteorTexture;

static Sound meteorImpact;

const int speedMeteros = 2;
const int maxBigMeteors = 50;
const int maxMidMeteors = 100;
const int maxSmallMeteors = 200;
static int setupBigmeteor = 0;
static int setupMidmeteor = 0;
static int setupSmallmeteor = 0;
static int score = 0;
static int maxScore = 0;

static Meteor bigMeteor[maxBigMeteors];
static Meteor mediumMeteor[maxMidMeteors];
static Meteor smallMeteor[maxSmallMeteors];

static int midMeteorsCount = 0;
static int smallMeteorsCount = 0;
static int destroyedMeteorsCount = 0;

void ColisionMeteors();
void LogicMeteor();
void SetupMeteor(bool isVictory);
void DrawMeteors();