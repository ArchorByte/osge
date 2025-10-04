#include "env.displays.hpp"

#include "../logs/logs.handler.hpp"
#include "../helpers/help.text.format.hpp"

#include <SDL3/SDL.h>
#include <vector>
#include <string>

// Retrieve and list all available displays.
// Warning: We assume that SDL3 has already been initialized. Normally, the engine takes care of that by default on startup.
std::vector<int> get_available_display_indexes()
{
    log("Retrieving all available displays on this device..");

    int displays_count = 0;
    SDL_DisplayID *displays_list = SDL_GetDisplays(&displays_count);

    // Prepare the output vector list.
    std::vector<int> output(displays_count);

    if (displays_list)
    {
        for (int i = 0; i < displays_count; ++i)
        {
            output.push_back(displays_list[i]);
        }

        SDL_free(displays_list);
    }
    else fatal_error_log("Failed to retrieve any displays on this device! " + std::string(SDL_GetError()) + ".");

    log(std::to_string(output.size()) + " displays were found!");
    return output;
}

// Retrieve and return the name of a specific display.
std::string get_display_name
(
    const int &display_index
)
{
    const char* display_name = SDL_GetDisplayName(display_index);

    if (!display_name)
    {
        error_log("Failed to retrieve the name of the display #" + std::to_string(display_index) + "! Returned \"Unknown Display\" by default.");
        return "Unknown Display";
    }

    return std::string(display_name);
}
