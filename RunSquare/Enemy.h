#pragma once

#include <tuple>
#include <cmath>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Enemy
{
private:
    SDL_Texture* texture;
    SDL_Rect position;
    
    const SDL_Rect* targetPosition;
    bool targetHit;

    float speed;
    bool active;

    int spawnTick;
    int lastSpeedUpdateTick;
public:
    Enemy(SDL_Renderer* renderer);
    ~Enemy();

    void Render(SDL_Renderer* renderer);
    void Update(SDL_Renderer* renderer, int deltaFrameTicks, int totalFrameCount, int totalTickCount);

    bool HasHitTarget();

    void SetTargetPositionPointer(const SDL_Rect* targetPosition);
    
    std::tuple<int, int> GetPosition();
};
