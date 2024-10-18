#include <filesystem>
void setWorkDir()
{
#if defined(_WIN32) || defined(__linux__)
    std::filesystem::current_path("../assets");
#endif
}