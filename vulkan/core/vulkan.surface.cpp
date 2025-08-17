#include "vulkan.surface.hpp"

#include "../../logs/logs.handler.hpp"
#include "../../utils/tool.text.format.hpp"

#include <vulkan/vulkan.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

// Create a Vulkan surface for a GLFW window.
VkSurfaceKHR create_vulkan_surface
(
    const VkInstance &vulkan_instance,
    SDL_Window* &window
)
{
    log("Creating a Vulkan surface..");

    if (!vulkan_instance || vulkan_instance == VK_NULL_HANDLE)
    {
        fatal_error_log("Vulkan surface creation failed! The Vulkan instance provided (" + force_string(vulkan_instance) + ") is not valid!");
    }

    if (!window)
    {
        fatal_error_log("Vulkan surface creation failed! The GLFW window provided (" + force_string(window) + ") is not valid!");
    }

    VkSurfaceKHR vulkan_surface = VK_NULL_HANDLE;
    bool surface_creation = SDL_Vulkan_CreateSurface(window, vulkan_instance, &vulkan_surface);

    if (surface_creation != SDL_TRUE)
    {
        fatal_error_log("Vulkan surface creation returned error code " + std::string(SDL_GetError()) + ".");
    }

    if (!vulkan_instance || vulkan_surface == VK_NULL_HANDLE)
    {
        fatal_error_log("Vulkan surface creation output \"" + force_string(vulkan_instance) + "\" is not valid!");
    }

    log("Vulkan surface " + force_string(vulkan_surface) + " created successfully!");
    return vulkan_surface;
}

// Destroy a Vulkan surface.
void destroy_vulkan_surface
(
    const VkInstance &vulkan_instance,
    VkSurfaceKHR &vulkan_surface
)
{
    log("Destroying the " + force_string(vulkan_surface) + " Vulkan surface..");

    if (!vulkan_instance || vulkan_instance == VK_NULL_HANDLE)
    {
        error_log("Vulkan surface destruction failed! The Vulkan instance provided (" + force_string(vulkan_instance) + ") is not valid!");
        return;
    }

    if (!vulkan_surface || vulkan_surface == VK_NULL_HANDLE)
    {
        error_log("Vulkan surface destruction failed! The Vulkan surface provided (" + force_string(vulkan_surface) + ") is not valid!");
        return;
    }

    // Destroy the surface and dispose of the address.
    vkDestroySurfaceKHR(vulkan_instance, vulkan_surface, nullptr);
    vulkan_surface = VK_NULL_HANDLE;

    log("Vulkan surface destroyed successfully!");
}

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

// Constructor.
Vulkan_Surface::Vulkan_Surface
(
    const VkInstance &vulkan_instance,
    SDL_Window* &window
) : vulkan_instance(vulkan_instance)
{
    vulkan_surface = create_vulkan_surface(vulkan_instance, window);
}

// Destructor.
Vulkan_Surface::~Vulkan_Surface()
{
    destroy_vulkan_surface(vulkan_instance, vulkan_surface);
}

VkSurfaceKHR Vulkan_Surface::get() const
{
    return vulkan_surface;
}
