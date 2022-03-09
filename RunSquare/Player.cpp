#include "Player.h"

const float INITIAL_PLAYER_SPEED = 0.20f;

Player::Player(SDL_Renderer *renderer)
{
    SDL_Surface* playerSurface = IMG_Load("/app0/assets/images/player.tga");
    this->texture = SDL_CreateTextureFromSurface(renderer, playerSurface);

    this->position.x = rand() % (1920 - playerSurface->w / 2);
    this->position.y = rand() % (1280 - playerSurface->h / 2);
    
    this->movementVector.x = 0;
    this->movementVector.y = 0;

    this->speed = INITIAL_PLAYER_SPEED;
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

void Player::Update(SDL_Renderer* renderer, int deltaFrameTicks, int totalFrameCount) {
    int movementX = 0;
    int movementY = 0;
    
    if (this->movementVector.x != 0) {
        movementX = this->speed * this->movementVector.x * deltaFrameTicks;
    }

    if (this->movementVector.y != 0) {
        movementY = this->speed * this->movementVector.y * deltaFrameTicks;
    }

    this->position.x += movementX;
    this->position.y += movementY;
}
                    

std::tuple<int, int> Player::GetPosition()
{
    return std::make_tuple(this->position.x, this->position.y);
}

void Player::SetMovement(int x, int y)
{
    this->movementVector.x = x;
    this->movementVector.y = y;
}
