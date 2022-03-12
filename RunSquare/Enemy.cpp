#include "Enemy.h"

const static float INITIAL_ENEMY_SPEED = 0.075f;
const static float INCREMENT_ENEMY_SPEED = 0.025f;

Enemy::Enemy(SDL_Renderer* renderer)
{
    SDL_Surface* enemySurface = IMG_Load("/app0/assets/images/enemy.tga");
    this->texture = SDL_CreateTextureFromSurface(renderer, enemySurface);
    SDL_SetTextureBlendMode(this->texture, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(this->texture, 128);

    this->position.x = rand() % (1920 - enemySurface->w);
    this->position.y = rand() % (1080 - enemySurface->h);

    this->targetHit = false;

    this->speed = INITIAL_ENEMY_SPEED;
    this->active = false;

    this->spawnTick = 0;
    this->lastSpeedUpdateTick = 0;
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
        if (this->spawnTick == 0) {
            this->spawnTick = totalTickCount;
        }

        if ((totalTickCount - this->spawnTick) >= 2000) {
            SDL_SetTextureAlphaMod(this->texture, 255);
            this->active = true;
        }
        else {
            return;
        }
    }
    
    if (SDL_HasIntersection(&this->position, this->targetPosition)) {
        this->targetHit = true;
        return;
    }

    if ((totalTickCount - this->lastSpeedUpdateTick) >= 10000) {
        this->speed += INCREMENT_ENEMY_SPEED;
        this->lastSpeedUpdateTick = totalTickCount;
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

bool Enemy::HasHitTarget()
{
    return this->targetHit;

}

void Enemy::SetTargetPositionPointer(const SDL_Rect* targetPosition)
{
    this->targetPosition = targetPosition;
}


std::tuple<int, int> Enemy::GetPosition()
{
    int x = 0;
    int y = 0;

    if (this->texture != nullptr)
    {
        x = this->position.x;
        y = this->position.y;
    }

    return std::make_tuple(x, y);
}
