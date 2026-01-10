#include "vulkan.core.hpp"
#include "osge/utils/utils.hpp"
#include <libraries/vulkan/vulkan.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

/*
    Create a Vulkan surface.
    Note: You should use the pre-made class to handle the Vulkan surface rather than directly using this function for memory safety reasons.

    Tasks:
        1) Verify the parameters.
        2) Create the Vulkan surface.

    Parameters:
        - vulkan_instance / VkInstance  / Instance of Vulkan.
        - window          / SDL_Window* / Targeted window.

    Returns:
        The created Vulkan surface.
*/
VkSurfaceKHR Vulkan::Core::create_vulkan_surface
(
    const VkInstance &vulkan_instance,
    SDL_Window* &window
)
{
    Utils::Logs::log("Creating a Vulkan surface..");

    if (vulkan_instance == VK_NULL_HANDLE)
        Utils::Logs::crash_error_log("Vulkan surface creation failed! The Vulkan instance provided (" + Utils::Text::get_memory_address(vulkan_instance) + ") is not valid!");

    if (!window)
        Utils::Logs::crash_error_log("Vulkan surface creation failed! The SDL3 window provided (" + Utils::Text::get_memory_address(window) + ") is not valid!");

    VkSurfaceKHR vulkan_surface = VK_NULL_HANDLE;
    const bool surface_creation = SDL_Vulkan_CreateSurface(window, vulkan_instance, nullptr, &vulkan_surface);

    if (!surface_creation)
        Utils::Logs::crash_error_log("Vulkan surface creation returned error code " + std::string(SDL_GetError()) + ".");

    Utils::Logs::log("Vulkan surface " + Utils::Text::get_memory_address(vulkan_surface) + " created successfully!");
    return vulkan_surface;
}



/*
    Destroy a Vulkan surface.

    Tasks:
        1) Verify the parameters.
        2) Destroy the Vulkan surface.
        3) Get rid of the object memory address.

    Parameters:
        - vulkan_instance / VkInstance   / Instance of Vulkan.
        - vulkan_surface  / VkSurfaceKHR / Vulkan surface to destroy.

    Returns:
        No object returned.
*/
void Vulkan::Core::destroy_vulkan_surface
(
    const VkInstance &vulkan_instance,
    VkSurfaceKHR &vulkan_surface
)
{
    Utils::Logs::log("Destroying the " + Utils::Text::get_memory_address(vulkan_surface) + " Vulkan surface..");

    if (vulkan_instance == VK_NULL_HANDLE)
    {
        Utils::Logs::error_log("Vulkan surface destruction failed! The Vulkan instance provided (" + Utils::Text::get_memory_address(vulkan_instance) + ") is not valid!");
        return;
    }

    if (vulkan_surface == VK_NULL_HANDLE)
    {
        Utils::Logs::error_log("Vulkan surface destruction failed! The Vulkan surface provided (" + Utils::Text::get_memory_address(vulkan_surface) + ") is not valid!");
        return;
    }

    vkDestroySurfaceKHR(vulkan_instance, vulkan_surface, nullptr);
    vulkan_surface = VK_NULL_HANDLE;

    Utils::Logs::log("Vulkan surface destroyed successfully!");
}

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

Vulkan::Core::vulkan_surface_handler::vulkan_surface_handler
(
    const VkInstance &vulkan_instance,
    SDL_Window* &window
)
    : vulkan_instance(vulkan_instance)
{
    vulkan_surface = Vulkan::Core::create_vulkan_surface(vulkan_instance, window);
}

Vulkan::Core::vulkan_surface_handler::~vulkan_surface_handler()
{
    Vulkan::Core::destroy_vulkan_surface(vulkan_instance, vulkan_surface);
}

VkSurfaceKHR Vulkan::Core::vulkan_surface_handler::get() const
{
    return vulkan_surface;
}
