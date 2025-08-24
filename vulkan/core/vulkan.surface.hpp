#include <vulkan/vulkan.h>
#include <SDL3/SDL.h>

#ifndef VULKAN_CORE_SURFACE_HPP
#define VULKAN_CORE_SURFACE_HPP

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

VkSurfaceKHR create_vulkan_surface
(
    const VkInstance &vulkan_instance,
    SDL_Window* &window
);

void destroy_vulkan_surface
(
    const VkInstance &vulkan_instance,
    VkSurfaceKHR &vulkan_surface
);

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

class Vulkan_Surface
{

public:
    // Constructor.
    Vulkan_Surface
    (
        const VkInstance &vulkan_instance,
        SDL_Window* &window
    );

    // Destructor.
    ~Vulkan_Surface();

    VkSurfaceKHR get() const;

    // Prevent data duplication.
    Vulkan_Surface(const Vulkan_Surface&) = delete;
    Vulkan_Surface& operator=(const Vulkan_Surface&) = delete;

private:
    // We declare the members of the class to store.
    VkInstance vulkan_instance = VK_NULL_HANDLE;
    VkSurfaceKHR vulkan_surface = VK_NULL_HANDLE;

};

#endif
