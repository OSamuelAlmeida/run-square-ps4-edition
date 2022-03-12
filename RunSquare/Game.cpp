#include <tuple>
#include <cmath>
#include <vector>

#include <orbis/libkernel.h>
#include <sys/resource.h>
#include <stdlib.h>
#include <time.h>

#include "Game.h"

const static float JOYSTICK_THRESHOLD = 20000.0;

FT_Face fontDebug;
FT_Face fontScore;

Color bgColor = { 0x00, 0x7D, 0xAF };
Color fgColor = { 255, 255, 255 };

SDL_Joystick* controller;

int lastLeftStickX = 0;
int lastLeftStickY = 0;

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
    Enemy* enemy = new Enemy(renderer);
    enemy->SetTargetPositionPointer(player->GetPositionPointer());
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
            if (gameOver || !gameOver)
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
        }
        break;
    }
}

void handleControllerAxisChange(SDL_Event* ev)
{
    double rotationAngle = 0;
    int moveX = 0;
    int moveY = 0;
    float rotateX = 0;
    float rotateY = 0;

    if (ev->jaxis.axis == 0) {
        rotateX = (float)ev->jaxis.value;
    }

    if (ev->jaxis.axis == 1) {
        rotateY = (float)ev->jaxis.value;
    }

    if (rotateX == 0.0f) {
        rotateX = lastLeftStickX;
    }
    else {
        lastLeftStickX = rotateX;
    }
    
    if (rotateY == 0.0f) {
        rotateY = lastLeftStickY;
    }
    else {
        lastLeftStickY = rotateY;
    }

    if (rotateX > JOYSTICK_THRESHOLD) {
        moveX = 1;
    }
    else if (rotateX < -JOYSTICK_THRESHOLD) {
        moveX = -1;
    }
    else {
        moveX = 0;
    }

    if (rotateY > JOYSTICK_THRESHOLD) {
        moveY = 1;
    }
    else if (rotateY < -JOYSTICK_THRESHOLD) {
        moveY = -1;
    }
    else {
        moveY = 0;
    }

    player->SetMovement(moveX, moveY);
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
            it->ResetCoin();

            hud->SetScore(currentScore);
        }
    }
}

void render(SDL_Renderer* renderer)
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

void update(SDL_Renderer *renderer, int deltaFrameTicks, int totalFrameCount, int totalTickCount)
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

void initGame(SDL_Renderer* renderer)
{
    srand(time(0));

    hud = new HUD();

    hud->SetColorInfo(fgColor, bgColor);
    hud->SetScore(0);

    player = new Player(renderer);
    spawnEnemy(renderer);
    spawnCoin(renderer);
}
