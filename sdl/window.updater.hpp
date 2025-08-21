#include <SDL2/SDL.h>

#ifndef SDL2_WINDOW_UPDATER_HPP
#define SDL2_WINDOW_UPDATER_HPP

void update_sdl_window
(
    SDL_Window* window,
    int new_window_mode,
    int new_width,
    int new_height,
    int new_refresh_rate,
    int new_display_index
);

#endif
