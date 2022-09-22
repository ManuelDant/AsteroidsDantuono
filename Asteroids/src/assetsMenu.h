#pragma once
#include "raylib.h"

static float scrollingBackMenu = 0.0f;

static Music menubg;
static Texture2D title;
static Texture2D option;
static Texture2D bground;

void DrawTitle();
void DrawOptionPlay(bool isOn);
void DrawOptionRules(bool isOn);
void DrawOptionCredits(bool isOn);
void PlayMusicMenu(bool isOn);
void BackgroundMenu();
void LoadResourcesMenu();
void UnloadResourcesMenu();

