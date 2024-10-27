#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>

#include "platform.h"
#include <string>
#include <iostream>
using namespace std;
#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 960
#define GLOBAL_FPS 60

void binarizeSurface(SDL_Surface *surface, Uint8 threshold)
{
    // 确保表面可以锁定，通常对于软件表面，需要锁定才能操作像素数据
    SDL_LockSurface(surface);
    const SDL_PixelFormatDetails *format = SDL_GetPixelFormatDetails(surface->format);
    Uint32 *pixel = (Uint32 *)surface->pixels;

    int w = surface->w, h = surface->h;
    Uint8 v = 0, a = 0;
    for (int j = 0; j < h; j++)
        for (int i = 0; i < w; i++)
        {
            SDL_GetRGBA(pixel[j * w + i], format, nullptr, nullptr, nullptr, nullptr, &a);
            v = a;
            if (v < threshold)
                pixel[j * w + i] = SDL_MapRGBA(format, NULL, 0, 0, 0, 0);
            else
                pixel[j * w + i] = SDL_MapRGBA(format, NULL, 0xff, 0xff, 0xff, 0xff);
        }
    SDL_UnlockSurface(surface);
}

// 测试ttf使用
int main(int argc, char *argv[])
{
    SetWorkDir(argv[0]);

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    TTF_Init();
    Mix_Init(MIX_INIT_MP3);

    SDL_Window *window = SDL_CreateWindow("Deltatraveler Remake", SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Renderer *renderer = CreateRenderer(window);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xff);
    SDL_RenderClear(renderer);
    int factor = 1, fontsize = 16;
    TTF_Font *font = TTF_OpenFont("lang/zh_CN/font/unifont.otf", fontsize);
    if (!font)
        return 1;

    SDL_Texture *textglyph = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB32, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_SetRenderTarget(renderer, textglyph);
    int start = 0x5e00, end = 0x5e00 + 5000;
    cout << end - start << endl;
    int t1 = SDL_GetTicks();
    SDL_FRect *rect = (SDL_FRect *)malloc(sizeof(SDL_FRect) * (end - start + 1));
    int x = 0, y = 0;
    for (Uint32 ch = start; ch <= end; ch++)
    {
        SDL_Surface *surface = TTF_RenderGlyph_Solid(font, ch, {0xff, 0xff, 0xff, 0xff});
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        bool wrap = x + texture->w > SCREEN_WIDTH;
        if (wrap)
        {
            x = 0;
            y += texture->h;
        }
        rect[ch - start] = {1.0f * x, 1.0f * y, texture->w * 1.0f, texture->h * 1.0f};
        SDL_RenderTexture(renderer, texture, nullptr, &rect[ch - start]);
        x += rect[ch - start].w * factor;
        SDL_DestroySurface(surface);
        SDL_DestroyTexture(texture);
    }
    cout << SDL_GetTicks() - t1 << endl;
    SDL_Texture *buffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB32, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_SetRenderTarget(renderer, buffer);
    SDL_RenderClear(renderer);

    SDL_SetTextureScaleMode(textglyph, SDL_SCALEMODE_NEAREST);
    t1 = SDL_GetTicks();
    SDL_FRect factored_rect = {0, 0, 0, 0};
    x = 0, y = 0;
    for (Uint32 ch = start; ch <= end; ch++)
    {
        SDL_SetRenderTarget(renderer, buffer);
        bool wrap = (x + rect->w * factor > SCREEN_WIDTH);
        if (wrap)
        {
            x = 0;
            y += rect[ch - start].h * factor;
        }
        factored_rect = {1.0f * x, 1.0f * y, rect[ch - start].w * factor * 1.0f, rect[ch - start].h * factor * 1.0f};
        SDL_RenderTexture(renderer, textglyph, &rect[ch - start], &factored_rect);
        x += rect[ch - start].w * factor;
        SDL_SetRenderTarget(renderer, nullptr);
        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, buffer, nullptr, nullptr);
        SDL_RenderPresent(renderer);
        SDL_Delay(1);
    }
    cout << SDL_GetTicks() - t1 << endl;
    free(rect);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}