#include <SDL3/SDL.h>

#ifndef SDL3_WINDOW_UPDATER_HPP
#define SDL3_WINDOW_UPDATER_HPP

void update_sdl3_window
(
    SDL_Window* window,
    int new_window_mode,
    const int &new_width,
    int new_height,
    const int &new_refresh_rate,
    const int &new_display_index
);

#endif
