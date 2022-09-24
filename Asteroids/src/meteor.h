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
static int setupBigmeteor = 0;
static int setupMidmeteor = 0;
static int setupSmallmeteor = 0;
static int level = 1;

static int score = 0;
static int maxScore = 0;

static Meteor bigMeteor[50];
static Meteor mediumMeteor[100];
static Meteor smallMeteor[200];

static int midMeteorsCount = 0;
static int smallMeteorsCount = 0;
static int destroyedMeteorsCount = 0;

void ColisionMeteors();
void LogicMeteor();
void SetupMeteor(bool isVictory);
void DrawMeteors();