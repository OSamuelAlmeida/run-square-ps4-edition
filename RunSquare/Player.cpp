#include "Player.h"

const static float INITIAL_PLAYER_SPEED = 0.2f;
const static float INCREMENT_PLAYER_SPEED = 0.05f;

Player::Player(SDL_Renderer *renderer)
{
    SDL_Surface* playerSurface = IMG_Load("/app0/assets/images/player.tga");
    this->texture = SDL_CreateTextureFromSurface(renderer, playerSurface);

    this->position.x = std::rand() % (Constants::FRAME_WIDTH - playerSurface->w);
    this->position.y = std::rand() % (Constants::FRAME_HEIGHT - playerSurface->h);
    
    this->movementVector.x = 0;
    this->movementVector.y = 0;

    this->speed = INITIAL_PLAYER_SPEED;
    this->lastSpeedUpdateTick = 0;
}

Player::~Player()
{
     if (this->texture != nullptr)
    {
        SDL_DestroyTexture(this->texture);
        this->texture = nullptr;
    }
}

void Player::Render(SDL_Renderer* renderer)
{
    if (this->texture != nullptr)
    {
        SDL_QueryTexture(this->texture, NULL, NULL, &this->position.w, &this->position.h);
        SDL_RenderCopy(renderer, this->texture, NULL, &this->position);
    }
}

void Player::Update(SDL_Renderer* renderer, int deltaFrameTicks, int totalFrameCount, int totalTickCount) {
    int deltaX = 0;
    int deltaY = 0;

    if ((totalTickCount - this->lastSpeedUpdateTick) >= 10000) {
        this->speed += INCREMENT_PLAYER_SPEED;
        this->lastSpeedUpdateTick = totalTickCount;
    }
    
    if (this->movementVector.x != 0) {
        deltaX = this->speed * this->movementVector.x * deltaFrameTicks;
    }

    if (this->movementVector.y != 0) {
        deltaY = this->speed * this->movementVector.y * deltaFrameTicks;
    }

    int newPositionX = this->position.x + deltaX;
    int newPositionY = this->position.y + deltaY;

    if (newPositionX > 0 && newPositionX < Constants::FRAME_WIDTH - (this->position.w))
        this->position.x = newPositionX;

    if (newPositionY > 0 && newPositionY < Constants::FRAME_HEIGHT - (this->position.h))
        this->position.y = newPositionY;
}

const SDL_Rect* Player::GetPositionPointer()
{
    return &this->position;
}

void Player::SetMovement(int x, int y)
{
    this->movementVector.x = x;
    this->movementVector.y = y;
}
