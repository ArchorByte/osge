#include "vulkan.core.hpp"

#include "libraries/vulkan/vulkan.h"
#include "osge/utils/utils.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

/*
    Create a Vulkan surface.
    Note: You should use the pre-made class to handle the Vulkan surface rather than directly using this function for memory safety reasons.

    Tasks:
        1) Verify function parameters.
        2) Create the Vulkan surface.

    Parameters:
        - vulkan_instance / VkInstance  / Current instance of Vulkan.
        - window          / SDL_Window* / Window of the game.

    Returns:
        The created Vulkan surface.
*/
VkSurfaceKHR Core::create_vulkan_surface
(
    const VkInstance &vulkan_instance,
    SDL_Window* &window
)
{
    Utils::Logs::log("Creating Vulkan surface.. ", false);

    if (vulkan_instance == VK_NULL_HANDLE)
        Utils::Logs::crash_log("Failed! Vulkan instance invalid.");

    if (!window)
        Utils::Logs::crash_log("Failed! SDL3 window invalid -> " + Utils::Text::get_memory_address(window) + ".");

    VkSurfaceKHR vulkan_surface = VK_NULL_HANDLE;
    const bool surface_creation = SDL_Vulkan_CreateSurface(window, vulkan_instance, nullptr, &vulkan_surface);

    if (!surface_creation)
        Utils::Logs::crash_log("Failed! Creation returned error code -> " + std::string(SDL_GetError()) + ".");

    Utils::Logs::log("Done! Memory address -> " + Utils::Text::get_memory_address(vulkan_surface) + ".", true);
    return vulkan_surface;
}



/*
    Destroy a Vulkan surface.

    Tasks:
        1) Verify function parameters.
        2) Destroy the Vulkan surface.
        3) Set object to null.

    Parameters:
        - vulkan_instance / VkInstance   / Current instance of Vulkan.
        - vulkan_surface  / VkSurfaceKHR / Vulkan surface to destroy.

    Returns:
        No object returned.
*/
void Core::destroy_vulkan_surface
(
    const VkInstance &vulkan_instance,
    VkSurfaceKHR &vulkan_surface
)
{
    Utils::Logs::log("Destroying Vulkan surface (" + Utils::Text::get_memory_address(vulkan_surface) + ").. ", false);

    if (vulkan_instance == VK_NULL_HANDLE)
    {
        Utils::Logs::log("Failed! Vulkan instance invalid.", true);
        return;
    }

    if (vulkan_surface == VK_NULL_HANDLE)
    {
        Utils::Logs::log("Failed! Vulkan surface invalid.", true);
        return;
    }

    vkDestroySurfaceKHR(vulkan_instance, vulkan_surface, nullptr);
    vulkan_surface = VK_NULL_HANDLE;

    Utils::Logs::log("Done!", true);
}

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

Core::vulkan_surface_handler::vulkan_surface_handler
(
    const VkInstance &vulkan_instance,
    SDL_Window* &window
)
    : vulkan_instance(vulkan_instance)
{
    vulkan_surface = Core::create_vulkan_surface(vulkan_instance, window);
}

Core::vulkan_surface_handler::~vulkan_surface_handler()
{
    Core::destroy_vulkan_surface(vulkan_instance, vulkan_surface);
}

VkSurfaceKHR Core::vulkan_surface_handler::get() const
{
    return vulkan_surface;
}
