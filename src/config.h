#pragma once
#include "state.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>

#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#endif

inline void EnsureConfigDir()
{
#ifdef __EMSCRIPTEN__
    return;
#elif defined(_WIN32)
    const char *appdata = getenv("APPDATA");
    if (!appdata) return;
    char dir[1024];
    snprintf(dir, sizeof(dir), "%s\\rainbow-triangle", appdata);
    _mkdir(dir);
#else
    const char *home = getenv("HOME");
    if (!home) return;
    char dir[1024];
#ifdef __APPLE__
    snprintf(dir, sizeof(dir), "%s/Library/Application Support/rainbow-triangle", home);
#else
    snprintf(dir, sizeof(dir), "%s/.config/rainbow-triangle", home);
#endif
    mkdir(dir, 0755);
#endif
}

inline const char *GetConfigPath()
{
    static char path[1024];
#ifdef __EMSCRIPTEN__
    snprintf(path, sizeof(path), "config.dat");
#elif defined(_WIN32)
    const char *appdata = getenv("APPDATA");
    snprintf(path, sizeof(path), "%s\\rainbow-triangle\\config.dat", appdata ? appdata : ".");
#elif defined(__APPLE__)
    const char *home = getenv("HOME");
    snprintf(path, sizeof(path), "%s/Library/Application Support/rainbow-triangle/config.dat", home ? home : ".");
#else
    const char *home = getenv("HOME");
    snprintf(path, sizeof(path), "%s/.config/rainbow-triangle/config.dat", home ? home : ".");
#endif
    return path;
}

inline void SaveConfig(const GameState &state)
{
    EnsureConfigDir();
    const char *cfgPath = GetConfigPath();
    FILE *f = fopen(cfgPath, "w");
    if (!f) return;
    fprintf(f, "safeMode=%d\n", state.safeMode ? 1 : 0);
    fprintf(f, "warningShown=%d\n", state.warningShown ? 1 : 0);
    fprintf(f, "musicEnabled=%d\n", state.musicEnabled ? 1 : 0);
    fprintf(f, "musicLevel=%d\n", state.musicLevel);
    fprintf(f, "volumeLevel=%d\n", state.volumeLevel);
    fprintf(f, "difficultyLevel=%d\n", state.difficultyLevel);
    fclose(f);
}

inline void LoadConfig(GameState &state)
{
    const char *cfgPath = GetConfigPath();
    FILE *f = fopen(cfgPath, "r");
    if (!f) return;
    char key[32];
    int value;
    while (fscanf(f, "%31[^=]=%d\n", key, &value) == 2)
    {
        if (strcmp(key, "safeMode") == 0) state.safeMode = (value != 0);
        else if (strcmp(key, "warningShown") == 0) state.warningShown = (value != 0);
        else if (strcmp(key, "musicEnabled") == 0) state.musicEnabled = (value != 0);
        else if (strcmp(key, "musicLevel") == 0) state.musicLevel = value;
        else if (strcmp(key, "volumeLevel") == 0) state.volumeLevel = value;
        else if (strcmp(key, "difficultyLevel") == 0) state.difficultyLevel = value;
    }
    fclose(f);
}
