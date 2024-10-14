#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define GLOBAL_FPS 60

namespace gameflag
{
    int counter = 0;
}

using gameflag::counter;
using std::cerr;
using std::cout;
using std::endl;

int calcDelay(int t0, int t1, int f)
{
    int tick = int((t1 - t0) * f * 1.0 / 1000.0) + 1;
    return int(1000.0 / f * tick + t0 - t1);
}
int main(int argc, char *argv[])
{
    // init
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Deltatest", SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
    int t0 = 0, t1 = 0;
    SDL_Event event{};
    bool running = true;

    while (running)
    {
        if (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_EVENT_KEY_DOWN:
                if (event.key.scancode == SDL_SCANCODE_ESCAPE)
                    running = false;
                break;
            default:
                break;
            }
        }
        t0 = SDL_GetTicks();
        t1 = SDL_GetTicks();
        if (t1 - t0 < 1000 / GLOBAL_FPS)
            SDL_Delay(calcDelay(t0, t1, GLOBAL_FPS));
        counter++;
    }
    // init image and animation
    // counter
    // event -- change the sprites 1/2, per frame
    // event -- no input -- sprite 0
    // animation -- n frame -- cut -- return a list
    // quit -- gettticks
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}