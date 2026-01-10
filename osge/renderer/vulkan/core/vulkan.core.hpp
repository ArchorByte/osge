#ifndef VULKAN_CORE_HPP
#define VULKAN_CORE_HPP

#include <libraries/vulkan/vulkan.h>
#include <SDL3/SDL.h>
#include <vector>

namespace Vulkan::Core
{
    //////////////////////////////////
    ///// extensions.support.cpp /////
    //////////////////////////////////

    bool check_extensions_support
    (
        const VkPhysicalDevice &physical_device,
        const std::vector<const char *> &required_extensions
    );

    //////////////////////////////
    ///// layers.support.cpp /////
    //////////////////////////////

    bool check_layers_support
    (
        const std::vector<const char*> &layers
    );

    ///////////////////////////////
    ///// vulkan.instance.cpp /////
    ///////////////////////////////

    VkInstance create_vulkan_instance
    (
        const std::vector<const char*> &layers
    );

    void destroy_vulkan_instance
    (
        VkInstance &vulkan_instance
    );

    //////////////////////////////
    ///// vulkan.surface.cpp /////
    //////////////////////////////

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

    /////////////////////////////////////////////////
    //////////////////// Classes ////////////////////
    /////////////////////////////////////////////////

    ///////////////////////////////
    ///// vulkan.instance.cpp /////
    ///////////////////////////////

    class instance_handler
    {
        public:
            instance_handler
            (
                const std::vector<const char*> &layers
            );

            ~instance_handler();
            VkInstance get() const;

            // Prevent data duplication.
            instance_handler(const instance_handler&) = delete;
            instance_handler &operator = (const instance_handler&) = delete;
        private:
            VkInstance vulkan_instance = VK_NULL_HANDLE;
    };

    //////////////////////////////
    ///// vulkan.surface.cpp /////
    //////////////////////////////

    class vulkan_surface_handler
    {
        public:
            vulkan_surface_handler
            (
                const VkInstance &vulkan_instance,
                SDL_Window* &window
            );

            ~vulkan_surface_handler();
            VkSurfaceKHR get() const;

            // Prevent data duplication.
            vulkan_surface_handler(const vulkan_surface_handler&) = delete;
            vulkan_surface_handler& operator=(const vulkan_surface_handler&) = delete;
        private:
            VkInstance vulkan_instance = VK_NULL_HANDLE;
            VkSurfaceKHR vulkan_surface = VK_NULL_HANDLE;
    };
}

#endif
