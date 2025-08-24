#include <SDL3/SDL.h>

#ifndef SDL3_WINDOW_UPDATER_HPP
#define SDL3_WINDOW_UPDATER_HPP

void update_sdl3_window
(
    SDL_Window* window,
    int new_window_mode,
    int new_width,
    int new_height,
    int new_refresh_rate,
    int new_display_index
);

#endif
