#pragma once

#include <tuple>
#include <cmath>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Constants.h"

class Enemy
{
private:
    SDL_Texture* texture;
    SDL_Rect position;
    
    const SDL_Rect* targetPosition;
    bool targetHit;

    float speed;
    bool active;

    int tickTracker;
public:
    Enemy(SDL_Renderer* renderer, const SDL_Rect* targetPosition);
    ~Enemy();

    void Render(SDL_Renderer* renderer);
    void Update(SDL_Renderer* renderer, int deltaFrameTicks, int totalFrameCount, int totalTickCount);

    const bool HasHitTarget();
};
