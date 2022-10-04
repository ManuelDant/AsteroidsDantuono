#include "propellerAnimated.h"
#include "player.h"

void LoadResourcePropeller() {
	propeller = LoadTexture("rsc/propeller.png");
	frameWidth = (float)(propeller.width / numFrames);
	frameHeight = (float)(propeller.height / numLines);
	frameRec = { 0, 0, frameWidth, frameHeight };

}

void UnloadResourcePropeller() {
	UnloadTexture(propeller);
}

void PropellerSetup() {
    int currentFrame = 0;
    int currentLine = 0;

    Vector2 position = { 0.0f, 0.0f };

    bool active = false;
    int framesCounter = 0;
}

void PropellerLogic() {
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
    {
        active = true;

    }
    if (active)
    {
        framesCounter += 3;

        if (framesCounter > 2)
        {
            currentFrame += 3;

            if (currentFrame >= numFrames)
            {
                currentFrame = 0;
                currentLine += 1;

                if (currentLine >= numLines)
                {
                    currentLine = 0;
                    active = false;
                }
            }

            framesCounter = 0;
        }
    }

    frameRec.x = frameWidth * currentFrame;
    frameRec.y = frameHeight * currentLine;
}

void DrawPropeller(Player& player) {
    PropellerSetup();
    PropellerLogic();
    if (active)  DrawTexturePro(propeller, frameRec, { player.position.x, player.position.y, 50, 50 }, { (float)frameWidth - 30, (float)frameHeight - 50 }, player.rotation, WHITE);;
}
