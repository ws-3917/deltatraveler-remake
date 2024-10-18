#include <filesystem>
void setWorkDir()
{
#if defined(ANDROID)
    std::filesystem::current_path(".");
#elif defined(_WIN32) || defined(__linux__)
    std::filesystem::current_path("../assets");
#endif
}