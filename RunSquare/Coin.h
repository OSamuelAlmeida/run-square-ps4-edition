#pragma once

#include <tuple>
#include <cmath>
#include <cstdlib>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Constants.h"

class Coin
{
private:
    SDL_Texture* texture;
    SDL_Rect position;

    bool targetHit;

    const SDL_Rect* targetPosition;
public:
    Coin(SDL_Renderer* renderer);
    ~Coin();

    void Reset();    
    
    void Render(SDL_Renderer* renderer);
    void Update(SDL_Renderer* renderer, int deltaFrameTicks, int totalFrameCount, int totalTickCount);

    bool HasHitTarget();

    void SetTargetPositionPointer(const SDL_Rect* targetPosition);
};
