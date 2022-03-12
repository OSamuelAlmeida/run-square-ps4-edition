#include <tuple>
#include <cmath>
#include <vector>

#include <orbis/libkernel.h>
#include <sys/resource.h>
#include <stdlib.h>
#include <time.h>

#include "Game.h"

const static int JOYSTICK_THRESHOLD = 20000;

FT_Face fontDebug;
FT_Face fontScore;

Color bgColor = { 0x00, 0x7D, 0xAF };
Color fgColor = { 255, 255, 255 };

SDL_Joystick* controller;

int lastAxisRotationX = 0;
int lastAxisRotationY = 0;

int currentScore = 0;
int gameStage = 1;
bool gameOver = false;
bool handledNextDifficulty = true;

int lastGameStageChangeTick = 0;

HUD* hud;
Player* player;
std::vector<Enemy*> enemies;
std::vector<Coin*> coins;


void spawnEnemy(SDL_Renderer* renderer)
{
    Enemy* enemy = new Enemy(renderer, player->GetPositionPointer());
    enemies.push_back(enemy);
}

void spawnCoin(SDL_Renderer* renderer)
{
    Coin* coin = new Coin(renderer);
    coin->SetTargetPositionPointer(player->GetPositionPointer());
    coins.push_back(coin);
}

void handleControllerButtonPress(SDL_Renderer* renderer, SDL_Event* ev)
{
    switch (ev->jbutton.button)
    {
    case PAD_BUTTON_OPTIONS:
        {
            currentScore = 0;
            gameStage = 1;
            handledNextDifficulty = true;

            hud->SetScore(currentScore);

            for (auto enemy : enemies)               
                delete enemy;

            for (auto coin : coins)
                delete coin;
                
            enemies.clear();
            coins.clear();

            player = new Player(renderer);
            spawnEnemy(renderer);
            spawnCoin(renderer);
                
            gameOver = false;
        }
        break;
    }
}

void handleControllerAxisChange(SDL_Event* ev)
{
    int axisRotationX = 0;

    if (ev->jaxis.axis == 0) {
        axisRotationX = ev->jaxis.value;
    }

    int axisRotationY = 0;

    if (ev->jaxis.axis == 1) {
        axisRotationY = ev->jaxis.value;
    }

    if (axisRotationX == 0.0f) {
        axisRotationX = lastAxisRotationX;
    }
    else {
        lastAxisRotationX  = axisRotationX;
    }
    
    if (axisRotationY == 0.0f) {
        axisRotationY = lastAxisRotationY;
    }
    else {
        lastAxisRotationY = axisRotationY;
    }

    int movementX = 0;
    
    if (axisRotationX != 0 && std::abs(axisRotationX) >= JOYSTICK_THRESHOLD) {
        movementX = axisRotationX / std::abs(axisRotationX);
    }

    int movementY = 0;

    if (axisRotationY != 0 && std::abs(axisRotationY) >= JOYSTICK_THRESHOLD) {
        movementY = axisRotationY / std::abs(axisRotationY);
    }

    player->SetMovement(movementX, movementY);
}

void checkCollisionUpdates()
{
    for (auto it : enemies) {
        if (it->HasHitTarget()) {
            gameOver = true;
            return;
        }
    }

    for (auto it : coins) {
        if (it->HasHitTarget()) {
            currentScore += 1;
            it->Reset();

            hud->SetScore(currentScore);
        }
    }
}

void Game::Render(SDL_Renderer* renderer)
{
    if (!gameOver)
    {
        player->Render(renderer);

        for (auto it : enemies)
            it->Render(renderer);

        for (auto it : coins)
            it->Render(renderer);
    }

    hud->Render(renderer);
}

void Game::Update(SDL_Renderer *renderer, int deltaFrameTicks, int totalFrameCount, int totalTickCount)
{
    SDL_Event ev;

    while (SDL_PollEvent(&ev) != 0)
    {
        if (ev.type == SDL_JOYBUTTONDOWN)
            handleControllerButtonPress(renderer, &ev);

        if (ev.type == SDL_JOYAXISMOTION)
            handleControllerAxisChange(&ev);
    }


    hud->Update(renderer, deltaFrameTicks, totalFrameCount, gameOver);

    if (!gameOver)
    {
        if ((totalTickCount - lastGameStageChangeTick) >= 10000) {
            handledNextDifficulty = false;
            gameStage += 1;
            lastGameStageChangeTick = totalTickCount;
        }

        if (gameStage % 2 == 0 && !handledNextDifficulty) {
            spawnEnemy(renderer);
            spawnCoin(renderer);
            handledNextDifficulty = true;
        }
        
        player->Update(renderer, deltaFrameTicks, totalFrameCount, totalTickCount);

        for (auto enemy : enemies)
            enemy->Update(renderer, deltaFrameTicks, totalFrameCount, totalTickCount);

        for (auto coin : coins)
            coin->Update(renderer, deltaFrameTicks, totalFrameCount, totalTickCount);

        checkCollisionUpdates();
    }
}

Game::Game(SDL_Renderer* renderer)
{
    srand(time(0));

    hud = new HUD();

    hud->SetColorInfo(fgColor, bgColor);
    hud->SetScore(0);

    player = new Player(renderer);
    spawnEnemy(renderer);
    spawnCoin(renderer);
}
