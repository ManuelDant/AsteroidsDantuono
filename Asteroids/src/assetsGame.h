#pragma once
#include "raylib.h"

static float scrollingBack = 0.0f;
static Texture2D ship;
static Texture2D shootexture;
static Texture2D mira;
static Texture2D bgroundgame;

static Sound shipShoot;
static Sound shipCrash;

static Music background;

void BackgroundGame();
void LoadResourcesGame();
void UnloadResourcesGame();
void CheckDebug(bool check);