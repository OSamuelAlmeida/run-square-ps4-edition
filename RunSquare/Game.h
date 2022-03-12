#pragma once

#include <cmath>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Color.h"
#include "TTF.h"

#include "HUD.h"
#include "Player.h"
#include "Coin.h"
#include "Enemy.h"

extern Color bgColor;
extern Color fgColor;

extern FT_Face fontDebug;
extern FT_Face fontScore;

extern SDL_Joystick* controller;

enum ControllerButton {
	PAD_BUTTON_OPTIONS = 9
};

class Game {
public:
    Game(SDL_Renderer* renderer);
    
    void Render(SDL_Renderer* renderer);
    void Update(SDL_Renderer* renderer, int deltaFrameTicks, int totalFrameCount, int totalTickCount);
};
