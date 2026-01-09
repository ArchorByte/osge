#ifndef VULKAN_QUEUES_HPP
#define VULKAN_QUEUES_HPP

#include <vector>
#include <vulkan/vulkan.h>

namespace Vulkan::Queues
{
    //////////////////////////////
    ///// command.pool.cpp /////
    //////////////////////////////

    VkCommandPool create_command_pool
    (
        const uint32_t &graphics_family_index,
        const VkDevice &logical_device
    );

    void destroy_command_pool
    (
        VkCommandPool &command_pool,
        const VkDevice &logical_device
    );

    //////////////////////////////
    ///// graphics.queue.cpp /////
    //////////////////////////////

    uint32_t get_graphics_queue_family_index
    (
        const std::vector<VkQueueFamilyProperties> &queue_families
    );

    /////////////////////////////
    ///// present.queue.cpp /////
    /////////////////////////////

    uint32_t get_present_queue_family_index
    (
        const VkPhysicalDevice &physical_device,
        const std::vector<VkQueueFamilyProperties> &queue_families,
        const VkSurfaceKHR &vulkan_surface
    );

    //////////////////////////////
    ///// queues.handler.cpp /////
    //////////////////////////////

    std::vector<VkQueueFamilyProperties> get_queue_families
    (
        const VkPhysicalDevice &physical_device
    );

    std::vector<VkDeviceQueueCreateInfo> make_queues_create_info
    (
        const VkPhysicalDevice &physical_device,
        const float &queue_priority,
        const std::vector<uint32_t> &required_queue_indexes
    );

    ///////////////////////////////////////////////
    //////////////////// Class ////////////////////
    ///////////////////////////////////////////////

    class command_pool_handler
    {
        public:
            command_pool_handler
            (
                const uint32_t &graphics_family_index,
                const VkDevice &logical_device
            );

            ~command_pool_handler();
            VkCommandPool get() const;

            // Prevent data duplication.
            command_pool_handler(const command_pool_handler&) = delete;
            command_pool_handler &operator = (const command_pool_handler&) = delete;
        private:
            VkCommandPool command_pool = VK_NULL_HANDLE;
            VkDevice logical_device = VK_NULL_HANDLE;
    };
}

#endif
