#include <SDL2/SDL.h>

#ifndef GLFW_UPDATER_HPP
#define GLFW_UPDATER_HPP

void update_sdl_window
(
    SDL_Window* window,
    int width,
    int height,
    int refresh_rate
);

#endif
