#include <sstream>
#include <orbis/libkernel.h>
#include <orbis/Sysmodule.h>

#include "Constants.h"
#include "Game.h"

#include "../../_common/log.h"

std::stringstream debugLogStream;

struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
} graphics;

struct {
    uint32_t lastFrameTicks  = 0;
    uint32_t startFrameTicks = 0;
    uint32_t deltaFrameTicks = 0;
    uint32_t frameCounter    = 0;
} frameInfo;

uint32_t initialAvailableDirectMemory;

int main(int argc, char* args[])
{
    int rc;
    SDL_Surface* windowSurface;

    setvbuf(stdout, NULL, _IONBF, 0);

    DEBUGLOG << "Initializing SDL";

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) != 0)
    {
        DEBUGLOG << "Failed to initialize SDL: " << SDL_GetError();
        for (;;);
    }

    // We have to force load the freetype module or we'll get unresolved NID crashes
    DEBUGLOG << "Initializing TTF";

    rc = sceSysmoduleLoadModule(0x009A);

    if (rc < 0)
    {
        DEBUGLOG << "Failed to load freetype module: " << std::string(strerror(errno));
        for (;;);
    }

    rc = FT_Init_FreeType(&ftLib);

    if (rc < 0)
    {
        for (;;);
    }

    const char* debugFontPath = "/app0/assets/fonts/VeraMono.ttf";

    DEBUGLOG << "Initializing debug font (" << debugFontPath << ")";

    if (!InitFont(&fontDebug, debugFontPath, 16))
    {
        DEBUGLOG << "Failed to initialize debug font '" << debugFontPath << "'";
        for (;;);
    }

    const char* scoreFontPath = "/app0/assets/fonts/Bebas-Regular.ttf";

    DEBUGLOG << "Initializing score font (" << scoreFontPath << ")";

    if (!InitFont(&fontScore, scoreFontPath, 40))
    {
        DEBUGLOG << "Failed to initialize score font '" << scoreFontPath << "'";
        for (;;);
    }

    DEBUGLOG << "Creating a window";

    graphics.window = SDL_CreateWindow("main",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              Constants::FRAME_WIDTH,
                              Constants::FRAME_HEIGHT,
                              0);

    if (!graphics.window)
    {
        DEBUGLOG << "Failed to create window: " << SDL_GetError();
        for (;;);
    }

    windowSurface = SDL_GetWindowSurface(graphics.window);
    graphics.renderer = SDL_CreateSoftwareRenderer(windowSurface);

    if (!graphics.renderer)
    {
        DEBUGLOG << "Failed to create software renderer: " << SDL_GetError();
        for (;;);
    }

    if (SDL_NumJoysticks() < 1)
    {
        DEBUGLOG << "No controllers available!";
        for (;;);
    }

    controller = SDL_JoystickOpen(0);

    if (controller == NULL)
    {
        DEBUGLOG << "Couldn't open controller handle: " << SDL_GetError();
        for (;;);
    }

    Game game(graphics.renderer);
    
    DEBUGLOG << "Entering draw loop...";

    for (int frame = 0;; frame++)
    {
        frameInfo.startFrameTicks = SDL_GetTicks();
        frameInfo.deltaFrameTicks = frameInfo.startFrameTicks - frameInfo.lastFrameTicks;
        frameInfo.lastFrameTicks  = frameInfo.startFrameTicks;

        SDL_SetRenderDrawColor(graphics.renderer, bgColor.r, bgColor.g, bgColor.b, 0xFF);
        SDL_RenderClear(graphics.renderer);

        game.Render(graphics.renderer);

        SDL_UpdateWindowSurface(graphics.window);

        game.Update(graphics.renderer,
               frameInfo.deltaFrameTicks,
               frameInfo.frameCounter++,
               frameInfo.startFrameTicks);
    }

    SDL_Quit();
    return 0;
}
