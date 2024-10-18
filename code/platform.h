#include <filesystem>
SDL_Renderer *testrenderer = nullptr;

// 设置工作目录的跨平台函数
void SetWorkDir()
{
#if defined(ANDROID)
    std::filesystem::current_path(".");
#elif defined(__linux__) || defined(_WIN32) || defined(_WIN64)
    std::filesystem::current_path("../assets");
#endif
}

// 跨平台的渲染器创建逻辑
SDL_Renderer *CreateRenderer(SDL_Window *window)
{
    // 根据不同平台设置渲染器优先级
    const char *renderers[] = {
#if defined(ANDROID)
        "vulkan",
        "opengles",
        "opengles2",
#elif defined(_WIN32) || defined(_WIN64)
        "direct3d12",
        "vulkan",
        "direct3d11",
        "opengl",
#elif defined(__linux__)
        "vulkan",
        "opengl",
#endif

        "software",
    };

    // 尝试按照优先级创建渲染器
    for (const char *renderer : renderers)
    {
        testrenderer = SDL_CreateRenderer(window, renderer);
        if (testrenderer)
        {
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "兼容渲染器", renderer, NULL);
            return testrenderer;
        }
    }
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "错误", "未找到兼容的渲染器！", NULL);
    return nullptr;
}