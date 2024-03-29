#include "Coin.h"

Coin::Coin(SDL_Renderer* renderer)
{
    SDL_Surface* coinSurface = IMG_Load("/app0/assets/images/coin.tga");
    this->texture = SDL_CreateTextureFromSurface(renderer, coinSurface);

    this->Reset();
}

Coin::~Coin()
{
    if (this->texture != nullptr)
    {
        SDL_DestroyTexture(this->texture);
        this->texture = nullptr;
    }
}

void Coin::Reset() {
    SDL_QueryTexture(this->texture, NULL, NULL, &this->position.w, &this->position.h);
    this->position.x = std::rand() % (Constants::FRAME_WIDTH - this->position.w);
    this->position.y = std::rand() % (Constants::FRAME_HEIGHT - this->position.h);
    this->targetHit = false;
}

void Coin::Render(SDL_Renderer* renderer)
{
    if (this->texture != nullptr)
    {
        SDL_QueryTexture(this->texture, NULL, NULL, &this->position.w, &this->position.h);
        SDL_RenderCopy(renderer, this->texture, NULL, &this->position);
    }
}

void Coin::Update(SDL_Renderer* renderer, int deltaFrameTicks, int totalFrameCount, int totalTickCount)
{
    if (SDL_HasIntersection(&this->position, this->targetPosition)) {
        this->targetHit = true;
    }
}

bool Coin::HasHitTarget()
{
    return this->targetHit;
}

void Coin::SetTargetPositionPointer(const SDL_Rect* targetPosition)
{
    this->targetPosition = targetPosition;
}
