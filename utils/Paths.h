//
// Created by intheloop on 12.01.2024.
//

#ifndef RAYGAME_PATHS_H
#define RAYGAME_PATHS_H

#include <string>
#include <filesystem>

#ifdef _WIN32
// Windows headers
    #include <windows.h>
#elif __APPLE__
// macOS headers
    #include <mach-o/dyld.h>
#elif __linux__
// Linux headers
#include <unistd.h>
#include <climits>
#else
#error "Platform not supported"
#endif

/*
 * implementing path operations for accessing them easier.
 * there can appear many problems especially when the executable is run from another directory.
 * */
namespace engine::paths {
    namespace fs = std::filesystem;

    static std::string getExecutablePath() {
        char path[PATH_MAX];
        #ifdef _WIN32
            GetModuleFileNameA(NULL, path, PATH_MAX);
        #elif __APPLE__
            uint32_t size = PATH_MAX;
            _NSGetExecutablePath(path, &size);
        #elif __linux__
            readlink("/proc/self/exe", path, PATH_MAX);
        #endif
        return std::string{ path };
    }

    static std::string getExecutableDir() {
        std::string path = getExecutablePath();
        return path.substr(0, path.find_last_of("\\/"));
    }

    inline const fs::path RESOURCES_DIR = fs::path(getExecutableDir()).append("resources");
    inline const fs::path FONTS_DIR = RESOURCES_DIR / "fonts";
    inline const fs::path IMAGES_PATH = RESOURCES_DIR / "images";
    inline const fs::path WOLFSTEIN_PATH = IMAGES_PATH / "wolftex" / "pics";
    inline const fs::path RETRO_PATH = IMAGES_PATH / "retro";
}

#endif //RAYGAME_PATHS_H
