#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include "platform.h"
#include <string>
#include <iostream>
using namespace std;
#define SCREEN_WIDTH 1280
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
    SetWorkDir();

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    TTF_Init();

    SDL_Window *window = SDL_CreateWindow("Deltatraveler Remake", SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Renderer *renderer = CreateRenderer(window);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xff);
    SDL_RenderClear(renderer);

    TTF_Font *font = TTF_OpenFont("lang/zh_CN/font/fzxs12-Pixel.ttf", 12);
    if (!font)
        return 1;
    Uint32 ch = U'测';
    const int scaler = 8;
    SDL_Surface *surface = TTF_GetGlyphImage(font, ch);
    binarizeSurface(surface, 0x7f);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
    SDL_FRect rect = {0, 0, (float)(texture->w * scaler), (float)(texture->h * scaler)};
    SDL_RenderTexture(renderer, texture, nullptr, &rect);
    texture = IMG_LoadTexture(renderer, "image/owCh/susie/normalD.png");
    rect.h = (float)(texture->h), rect.w = (float)(texture->w);
    SDL_RenderTexture(renderer, texture, nullptr, &rect);
    SDL_RenderPresent(renderer);

    bool running = true;
    SDL_Delay(2000);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}