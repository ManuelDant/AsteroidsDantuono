#pragma once
#include "raylib.h"

static int numFrames = 10;
static int numLines = 6;

static int currentFrame;
static int currentLine;

static Vector2 position;
static bool active;
static int framesCounter;

static Texture2D propeller;

static float frameWidth;
static float frameHeight;
static Rectangle frameRec;

void PropellerLogic();
void PropellerSetup();
void DrawPropeller();