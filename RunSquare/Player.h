#pragma once

#include <tuple>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Player
{
private:
    SDL_Texture* texture;
    SDL_Rect position;
    SDL_Rect movementVector;
    
    float speed;
    int lastSpeedUpdateTick;
public:
    Player(SDL_Renderer *renderer);
    ~Player();

    void Render(SDL_Renderer* renderer);
    void Update(SDL_Renderer* renderer, int deltaFrameTicks, int totalFrameCount, int totalTickCount);

    const SDL_Rect* GetPositionPointer();
    
    void SetMovement(int x, int y);
};
