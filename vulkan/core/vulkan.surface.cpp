#include "vulkan.core.hpp"

#include "../../logs/logs.handler.hpp"
#include "../../utils/tool.text.format.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <vulkan/vulkan.h>

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
    log("Creating a Vulkan surface..");

    if (vulkan_instance == VK_NULL_HANDLE)
        fatal_error_log("Vulkan surface creation failed! The Vulkan instance provided (" + force_string(vulkan_instance) + ") is not valid!");

    if (!window)
        fatal_error_log("Vulkan surface creation failed! The SDL3 window provided (" + force_string(window) + ") is not valid!");

    VkSurfaceKHR vulkan_surface = VK_NULL_HANDLE;
    const bool surface_creation = SDL_Vulkan_CreateSurface(window, vulkan_instance, nullptr, &vulkan_surface);

    if (!surface_creation)
        fatal_error_log("Vulkan surface creation returned error code " + std::string(SDL_GetError()) + ".");

    log("Vulkan surface " + force_string(vulkan_surface) + " created successfully!");
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
    log("Destroying the " + force_string(vulkan_surface) + " Vulkan surface..");

    if (vulkan_instance == VK_NULL_HANDLE)
    {
        error_log("Vulkan surface destruction failed! The Vulkan instance provided (" + force_string(vulkan_instance) + ") is not valid!");
        return;
    }

    if (vulkan_surface == VK_NULL_HANDLE)
    {
        error_log("Vulkan surface destruction failed! The Vulkan surface provided (" + force_string(vulkan_surface) + ") is not valid!");
        return;
    }

    vkDestroySurfaceKHR(vulkan_instance, vulkan_surface, nullptr);
    vulkan_surface = VK_NULL_HANDLE;

    log("Vulkan surface destroyed successfully!");
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
