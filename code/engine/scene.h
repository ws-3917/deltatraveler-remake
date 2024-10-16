#include <SDL3/SDL.h>

#ifndef _SCENE_H_
#define _SCENE_H_
// scene class, for showing different "rooms", "menus"
class RT_Scene
{
public:
    RT_Scene() = default;
    ~RT_Scene() = default;

    virtual void enter()
    {
    }
    virtual void input(const SDL_Event)
    {
    }
    virtual void update()
    {
    }
    virtual void update()
    {
    }
    virtual void draw()
    {
    }
    virtual void exit()
    {
    }
};
#endif