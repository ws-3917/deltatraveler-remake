#include <iostream>
#include <vector>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include "platform.h" // 跨平台处理

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 960
#define GLOBAL_FPS 60
#define GLOBAL_ANIMATION_FPS 60
enum position
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    NONE
};
namespace RTgameflag
{
    int counter = 0;
    bool fullscreen = false;
    int keystate[4] = {0}; // up, down, left, right
    int keyPressed = 0;
}

using RTgameflag::counter;
using RTgameflag::fullscreen;
using RTgameflag::keyPressed;
using RTgameflag::keystate;
using std::cerr;
using std::cout;
using std::endl;
using std::vector;

// animation class
class RTanimation
{
public:
    RTanimation(SDL_Renderer *Renderer, const char *FileName, int FrameCount, int FrameGap_ms, int Scaling)
    {
        gap = FrameGap_ms;
        renderer = Renderer;
        framecount = FrameCount;
        SDL_Surface *surface = IMG_Load(FileName);
        SDL_Surface *resizedSurface = SDL_ScaleSurface(surface, surface->w * Scaling, surface->h * Scaling, SDL_SCALEMODE_NEAREST);
        texture = SDL_CreateTextureFromSurface(renderer, resizedSurface);
        SDL_DestroySurface(surface);
        SDL_DestroySurface(resizedSurface);

        SDL_GetTextureSize(texture, &width, &height);
        width /= FrameCount;

        sourceRect->h = height;
        sourceRect->w = width;
        distRect->h = height;
        distRect->w = width;
    }
    void present(float x, float y, int delta)
    {
        timer += delta;
        if (timer >= gap)
        {
            timer = timer % gap;
            ind_frame = (ind_frame + 1) % framecount;
        }
        distRect->x = x;
        distRect->y = y;
        sourceRect->x = ind_frame * width;
        sourceRect->y = 0;
        RTanimation::draw();
    }
    void present(float x, float y, int index, int delta)
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
    float width, height; // per frame
    ~RTanimation()
    {
        SDL_DestroyTexture(texture);
        delete sourceRect;
        delete distRect;
    }

private:
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
void PressKey(const int Direct)
{
    if (!keystate[Direct])
    {
        keystate[Direct] = keyPressed + 1;
        keyPressed++;
    }
}
void ReleaseKey(const int Direct)
{
    if (keystate[Direct])
    {
        for (int i = 0; i < 4; i++)
        {
            if (keystate[i] > keystate[Direct])
                keystate[i]--;
        }
        keystate[Direct] = 0;
        keyPressed--;
    }
}
int calcDelay(int t0, int t1, int f)
{
    int tick = int((t1 - t0) * f * 1.0 / 1000.0) + 1;
    return int(1000.0 / f * tick + t0 - t1);
}
int main(int argc, char *argv[])
{
    SetWorkDir();
    // init
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    IMG_Init(IMG_INIT_PNG);
    SDL_Window *window = SDL_CreateWindow("Deltatraveler Remake", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN);
    SDL_Renderer *renderer = CreateRenderer(window);
    SDL_Event event{};
    bool running = true;
    int t0 = 0, t1 = 0;
    int speed = 4;
    double x_shift = 0, y_shift = 0;
    position Susieface = DOWN;
    SDL_FPoint SusiePosition = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2}; // initial position
    vector<RTanimation *> animation = {
        new RTanimation(renderer, "SusieWalk_up.png", 4, 8 * 1000 / GLOBAL_ANIMATION_FPS, 3),
        new RTanimation(renderer, "SusieWalk_down.png", 4, 8 * 1000 / GLOBAL_ANIMATION_FPS, 3),
        new RTanimation(renderer, "SusieWalk_left.png", 4, 8 * 1000 / GLOBAL_ANIMATION_FPS, 3),
        new RTanimation(renderer, "SusieWalk_right.png", 4, 8 * 1000 / GLOBAL_ANIMATION_FPS, 3)};

    float touch_x = 0, touch_y = 0;
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
                case SDL_SCANCODE_UP:
                    PressKey(UP);
                    break;
                case SDL_SCANCODE_DOWN:
                    PressKey(DOWN);
                    break;
                case SDL_SCANCODE_LEFT:
                    PressKey(LEFT);
                    break;
                case SDL_SCANCODE_RIGHT:
                    PressKey(RIGHT);
                    break;
                default:
                    break;
                }
                break;
            case SDL_EVENT_KEY_UP:
                switch (event.key.scancode)
                {
                case SDL_SCANCODE_UP:
                    ReleaseKey(UP);
                    break;
                case SDL_SCANCODE_DOWN:
                    ReleaseKey(DOWN);
                    break;
                case SDL_SCANCODE_LEFT:
                    ReleaseKey(LEFT);
                    break;
                case SDL_SCANCODE_RIGHT:
                    ReleaseKey(RIGHT);
                    break;
                default:
                    break;
                }
                break;

            case SDL_EVENT_QUIT:
                running = false;
                break;
            case SDL_EVENT_FINGER_DOWN:
                touch_x = event.tfinger.x * SCREEN_WIDTH, touch_y = event.tfinger.y * SCREEN_HEIGHT;
                if (touch_x < SCREEN_WIDTH / 3.0)
                    PressKey(LEFT);
                else if (touch_x > SCREEN_WIDTH * 2.0 / 3.0)
                    PressKey(RIGHT);
                else if (touch_y < SCREEN_HEIGHT / 2.0)
                    PressKey(UP);
                else
                    PressKey(DOWN);
                break;
            case SDL_EVENT_FINGER_UP:
                touch_x = event.tfinger.x * SCREEN_WIDTH, touch_y = event.tfinger.y * SCREEN_HEIGHT;
                if (touch_x < SCREEN_WIDTH / 3.0)
                    ReleaseKey(LEFT);
                else if (touch_x > SCREEN_WIDTH * 2.0 / 3.0)
                    ReleaseKey(RIGHT);
                else if (touch_y < SCREEN_HEIGHT / 2.0)
                    ReleaseKey(UP);
                else
                    ReleaseKey(DOWN);
                break;
            default:
                break;
            }
        }
        x_shift = -bool(keystate[2] && SusiePosition.x > 0) + bool(keystate[3] && SusiePosition.x < SCREEN_WIDTH - animation[0]->width);
        y_shift = -bool(keystate[0] && SusiePosition.y > 0) + bool(keystate[1] && SusiePosition.y < SCREEN_HEIGHT - animation[0]->height);

        if (x_shift != 0 || y_shift != 0)
        {
            int i;
            for (i = 0; i < 3; i++)
            {
                if (keystate[i] == 1)
                    break;
            }
            Susieface = position(i);
            SusiePosition.x += speed * x_shift / SDL_sqrtf(x_shift * x_shift + y_shift * y_shift);
            SusiePosition.y += speed * y_shift / SDL_sqrtf(x_shift * x_shift + y_shift * y_shift);
            animation[Susieface]->present(SusiePosition.x, SusiePosition.y, SDL_GetTicks() - t1);
        }
        else
        {
            animation[Susieface]->present(SusiePosition.x, SusiePosition.y, 0, SDL_GetTicks() - t1);
        }
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
    for (int i = 0; i < animation.size(); i++)
    {
        delete animation[i];
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}