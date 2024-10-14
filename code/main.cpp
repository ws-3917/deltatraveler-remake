#include <iostream>
#include <vector>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define GLOBAL_FPS 240
#define GLOBAL_ANIMATION_FPS 60

namespace RTgameflag
{
    int counter = 0;
    bool fullscreen = false;
}

using RTgameflag::counter;
using RTgameflag::fullscreen;
using std::cerr;
using std::cout;
using std::endl;
using std::vector;

// animation class
class RTanimation
{
public:
    RTanimation(SDL_Renderer *Renderer, const char *FileName, int FrameCount, int FrameGap_ms)
    {
        gap = FrameGap_ms;
        renderer = Renderer;
        framecount = FrameCount;
        texture = IMG_LoadTexture(renderer, FileName);

        SDL_GetTextureSize(texture, &width, &height);
        width /= FrameCount;

        sourceRect->h = height;
        sourceRect->w = width;
        distRect->h = height;
        distRect->w = width;
    }
    void present(int x, int y, int delta)
    {
        timer += delta;
        if (timer >= gap)
        {
            timer = timer % gap;
            ind_frame = (ind_frame + 1) % framecount;
            distRect->x = x;
            distRect->y = y;
            sourceRect->x = ind_frame * width;
            sourceRect->y = 0;
            RTanimation::draw();
        }
    }
    void present(int x, int y, int index, int delta)
    {
        timer += delta;
        if (timer >= gap)
        {
            timer = timer % gap;
            ind_frame = (ind_frame + 1) % framecount;
        }
        distRect->x = x;
        distRect->y = y;
        sourceRect->x = index * width;
        sourceRect->y = 0;
        RTanimation::draw();
    }
    ~RTanimation()
    {
        SDL_DestroyTexture(texture);
        delete sourceRect;
        delete distRect;
    }

private:
    float width, height; // per frame
    int gap, framecount, timer = 0, ind_frame = 0;
    SDL_Texture *texture;
    SDL_FRect *sourceRect = new SDL_FRect(), *distRect = new SDL_FRect();
    SDL_Renderer *renderer;

    void draw()
    {
        // SDL_SetRenderTarget(renderer, texture);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, texture, sourceRect, distRect);
        SDL_RenderPresent(renderer);
    }
};

int calcDelay(int t0, int t1, int f)
{
    int tick = int((t1 - t0) * f * 1.0 / 1000.0) + 1;
    return int(1000.0 / f * tick + t0 - t1);
}
int main(int argc, char *argv[])
{
    // init
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    SDL_Window *window = SDL_CreateWindow("Deltatest", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

    SDL_Event event{};

    bool running = true;
    int t0 = 0, t1 = 0;

    RTanimation *animation = new RTanimation(renderer, "../assets/SusieWalk_down.png", 4, 4 * 1000 / GLOBAL_ANIMATION_FPS);

    // main loop
    while (running)
    {
        t0 = SDL_GetTicks();
        if (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_EVENT_KEY_DOWN:
                switch (event.key.scancode)
                {
                case SDL_SCANCODE_ESCAPE:
                    running = false;
                    break;

                case SDL_SCANCODE_F4:
                    fullscreen = !fullscreen;
                    SDL_SetWindowFullscreen(window, fullscreen);
                    break;
                default:
                    break;
                }
                break;
            case SDL_EVENT_QUIT:
                running = false;
                break;
            default:
                break;
            }
        }
        animation->present(SCREEN_HEIGHT / 2, SCREEN_WIDTH / 2, SDL_GetTicks() - t1);
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

    // 两个问题 -- 怎么进行整数缩放，怎么设置“图层“（多动画对象），静态图片的展示，以及移动对象的控制
    delete animation;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}