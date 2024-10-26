#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>

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
    SetWorkDir(argv[0]);

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    TTF_Init();
    Mix_Init(MIX_INIT_MP3);

    SDL_Window *window = SDL_CreateWindow("Deltatraveler Remake", SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Renderer *renderer = CreateRenderer(window);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xff);
    SDL_RenderClear(renderer);
    Mix_OpenAudio(0, NULL);

    Mix_Music *music = Mix_LoadMUS("audio/music/arms.mp3");
    Mix_PlayMusic(music, 0);

    Mix_Chunk *chunk = Mix_LoadWAV("audio/sound/sans.mp3");
    int fontsize = 120;
    TTF_Font *font = TTF_OpenFont("lang/zh_CN/font/fzxs12-Pixel.ttf", fontsize);
    if (!font)
        return 1;
    const int scaler = 8;
    SDL_Texture *textglyph = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB32, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_SetTextureScaleMode(textglyph, SDL_SCALEMODE_NEAREST);
    SDL_SetRenderTarget(renderer, textglyph);
    int t1 = SDL_GetTicks();
    SDL_FRect rect;
    for (Uint32 ch = 0x30; ch <= 0x60; ch++)
    {
        SDL_Surface *surface = TTF_RenderGlyph_Solid(font, ch, {0xff, 0xff, 0xff, 0xff});
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
        rect.x = fontsize * ((ch - 0x30) % (SCREEN_WIDTH / fontsize));
        rect.y = fontsize * ((ch - 0x30) / (SCREEN_WIDTH / fontsize));
        rect.w = texture->w * 1.0f;
        rect.h = texture->h * 1.0f;
        SDL_RenderTexture(renderer, texture, nullptr, &rect);
        SDL_DestroySurface(surface);
        SDL_DestroyTexture(texture);
    }

    SDL_SetRenderTarget(renderer, nullptr);
    cout << SDL_GetTicks() - t1 << endl;
    rect.w = 60;
    rect.h = 60;
    t1 = SDL_GetTicks();
    SDL_RenderClear(renderer);
    for (Uint32 ch = 0x30; ch <= 0x60; ch++)
    {
        rect.x = fontsize * ((ch - 0x30) % (SCREEN_WIDTH / fontsize));
        rect.y = fontsize * ((ch - 0x30) / (SCREEN_WIDTH / fontsize));
        SDL_RenderTexture(renderer, textglyph, &rect, &rect);
        SDL_RenderPresent(renderer);
    }
    cout << SDL_GetTicks() - t1 << endl;
    for (int i = 0; i < 5; i++)
    {
        Mix_PlayChannel(0, chunk, 0);
        SDL_Delay(100);
    }
    SDL_Delay(1000);
    Mix_CloseAudio();
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    Mix_Quit();

    return 0;
}