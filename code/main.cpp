#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <vector>

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Moving Triangle", 1280, 960, SDL_WINDOW_RESIZABLE);
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, "vulkan");

    // 定义原始的三角形顶点
    const std::vector<SDL_Vertex> origin_vertices = {
        SDL_Vertex{{150.0f, 100.0f}, {1.0f, 0.0f, 0.0f, 1.0f}}, // 顶点 (红色)
        SDL_Vertex{{0.0f, 300.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},   // 左下角 (绿色)
        SDL_Vertex{{300.0f, 300.0f}, {0.0f, 0.0f, 1.0f, 1.0f}}  // 右下角 (蓝色)
    };

    uint64_t last_ticks = SDL_GetTicks(); // 记录上一次的时间
    float position = 0.0f;                // 三角形的位置
    float direction = 1.0f;               // 方向 (1.0f 向右, -1.0f 向左)

    SDL_Event event{};
    bool keep_going = true;

    while (keep_going)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_EVENT_QUIT:
                keep_going = false;
                break;

            case SDL_EVENT_KEY_DOWN:
                keep_going = keep_going && (event.key.scancode != SDL_SCANCODE_ESCAPE);
                break;
            }
        }

        // 计算时间增量
        uint64_t current_ticks = SDL_GetTicks();
        float delta_time = (current_ticks - last_ticks) / 1000.0f; // 秒为单位
        last_ticks = current_ticks;

        // 更新位置
        position += 200.0f * delta_time * direction;

        // 获取窗口宽度，确保三角形不会跑出窗口边缘
        int width = 0;
        SDL_GetRenderOutputSize(renderer, &width, nullptr);
        float max_position = static_cast<float>(width) - (origin_vertices[2].position.x - origin_vertices[1].position.x);

        if (position > max_position)
        {
            direction = -1.0f; // 反向
        }
        else if (position < 0.0f)
        {
            position = 0.0f;
            direction = 1.0f;
        }

        // 修改顶点的坐标，实现水平移动
        std::vector<SDL_Vertex> vertices;
        for (const auto &vertex : origin_vertices)
        {
            vertices.push_back(vertex);
            vertices.back().position.x += position;
        }

        // 清除屏幕
        SDL_SetRenderDrawColor(renderer, 16, 0, 16, 255); // 深紫色背景
        SDL_RenderClear(renderer);

        // 渲染三角形
        SDL_RenderGeometry(renderer, nullptr, vertices.data(), vertices.size(), nullptr, 0);

        // 显示渲染内容
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
