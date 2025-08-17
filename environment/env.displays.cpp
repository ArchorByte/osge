#include "env.displays.hpp"

#include "../logs/logs.handler.hpp"
#include "../utils/tool.text.format.hpp"

#include <SDL2/SDL.h>
#include <vector>
#include <string>

// Retrieve and list all available displays.
// Warning: We assume that SDL2 has already been initialized! Normally, the engine takes care of that by default on startup.
std::vector<int> get_available_displays()
{
    log("Retrieving all available displays on this device..");
    std::vector<int> output;

    int displays_count = SDL_GetNumVideoDisplays();

    if (displays_count < 1)
    {
        fatal_error_log("No displays were found on this device! " + std::string(SDL_GetError()) + ".");
        return output; // Useless as fatal_error_log exits the app, but called anyway to avoid unnecessary compiler warnings.
    }

    // Register all displays in the list.
    for (int i = 0; i < displays_count; i++)
    {
        output.emplace_back(i);
    }

    log(std::to_string(output.size()) + " displays were found!");
    return output;
}

// Retrieve the name of a display.
std::string get_display_name
(
    int display_index
)
{
    if (display_index < 0 || display_index >= SDL_GetNumVideoDisplays())
    {
        error_log("Invalid display index provided (" + std::to_string(display_index) + ")! Returned \"Unknown display\" by default.");
        return "Unknown Display";
    }

    const char* display_name = SDL_GetDisplayName(display_index);

    if (!display_name)
    {
        error_log("Failed to retrieve the name of the display #" + std::to_string(display_index) + "! Returned \"Unknown display\" by default.");
        return "Unknown Display";
    }

    return std::string(display_name);;
}
