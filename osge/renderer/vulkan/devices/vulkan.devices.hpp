#ifndef VULKAN_DEVICES_HPP
#define VULKAN_DEVICES_HPP

#include <libraries/vulkan/vulkan.h>
#include <string>
#include <vector>

namespace Vulkan::Devices
{
    //////////////////////////////
    ///// logical.device.cpp /////
    //////////////////////////////

    VkDevice create_logical_device
    (
        const VkPhysicalDevice &physical_device,
        const std::vector<VkDeviceQueueCreateInfo> &queues_create_info,
        const std::vector<const char *> &required_extensions
    );

    void destroy_logical_device
    (
        VkDevice &logical_device
    );

    ///////////////////////////////
    ///// physical.device.cpp /////
    ///////////////////////////////

    bool is_valid_physical_device
    (
        const VkPhysicalDevice &physical_device
    );

    std::string get_physical_device_name
    (
        const VkPhysicalDevice &physical_device
    );

    VkPhysicalDevice select_physical_device
    (
        int &selected_gpu,
        const VkInstance &vulkan_instance
    );

    ///////////////////////////////////////////////
    //////////////////// Class ////////////////////
    ///////////////////////////////////////////////

    class logical_device_handler
    {
        public:
            logical_device_handler
            (
                const VkPhysicalDevice &physical_device,
                const std::vector<VkDeviceQueueCreateInfo> &queues_create_info,
                const std::vector<const char *> &required_extensions
            );

            ~logical_device_handler();
            VkDevice get() const;

            // Prevent data duplication.
            logical_device_handler(const logical_device_handler&) = delete;
            logical_device_handler &operator = (const logical_device_handler&) = delete;
        private:
            VkDevice logical_device = VK_NULL_HANDLE;
    };
}

#endif
