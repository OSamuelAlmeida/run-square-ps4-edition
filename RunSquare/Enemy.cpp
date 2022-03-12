#include "Enemy.h"

const static float INITIAL_ENEMY_SPEED = 0.075f;
const static float INCREMENT_ENEMY_SPEED = 0.025f;
const static float ACTIVATION_CYCLE_IN_MS = 2000.0f;
const static float SPEED_INCREMENT_CYCLE_IN_MS = 10000.0f;

Enemy::Enemy(SDL_Renderer* renderer, const SDL_Rect* targetPosition)
    : targetHit(false), speed(INITIAL_ENEMY_SPEED),
      active(false), tickTracker(0), targetPosition(targetPosition)
{
    SDL_Surface* enemySurface = IMG_Load("/app0/assets/images/enemy.tga");
    this->texture = SDL_CreateTextureFromSurface(renderer, enemySurface);
    
    SDL_SetTextureBlendMode(this->texture, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(this->texture, 128);

    this->position.x = rand() % (Constants::FRAME_WIDTH - enemySurface->w);
    this->position.y = rand() % (Constants::FRAME_HEIGHT - enemySurface->h);
}

Enemy::~Enemy()
{
    if (this->texture != nullptr)
    {
        SDL_DestroyTexture(this->texture);
        this->texture = nullptr;
    }
}

void Enemy::Render(SDL_Renderer* renderer)
{
    if (this->texture != nullptr)
    {
        SDL_QueryTexture(this->texture, NULL, NULL, &this->position.w, &this->position.h);
        SDL_RenderCopy(renderer, this->texture, NULL, &this->position);
    }
}

void Enemy::Update(SDL_Renderer* renderer, int deltaFrameTicks, int totalFrameCount, int totalTickCount)
{
    if (this->targetHit) {
        return;
    }
    
    if (!this->active) {
        if (this->tickTracker == 0) {
            this->tickTracker = totalTickCount;
        }

        if ((totalTickCount - this->tickTracker) >= ACTIVATION_CYCLE_IN_MS) {
            SDL_SetTextureAlphaMod(this->texture, 255);
            this->active = true;
            this->tickTracker = totalTickCount;
        }
        else {
            return;
        }
    }
    
    if (SDL_HasIntersection(&this->position, this->targetPosition)) {
        this->targetHit = true;
        return;
    }

    if ((totalTickCount - this->tickTracker) >= SPEED_INCREMENT_CYCLE_IN_MS) {
        this->speed += INCREMENT_ENEMY_SPEED;
        this->tickTracker = totalTickCount;
    }

    int targetDeltaX = this->targetPosition->x - this->position.x;
    int targetDeltaY = this->targetPosition->y - this->position.y;

    int movementX = 0;
    if (targetDeltaX != 0) {
        movementX = targetDeltaX / abs(targetDeltaX);
    }
    
    int movementY = 0;
    if (targetDeltaY != 0) {
        movementY = targetDeltaY / abs(targetDeltaY);
    }
    
    this->position.x += this->speed * movementX * deltaFrameTicks;
    this->position.y += this->speed * movementY * deltaFrameTicks;
}

const bool Enemy::HasHitTarget()
{
    return this->targetHit;

}
