#include <vulkan/vulkan.h>
#include <string>
#include <vector>

#ifndef VULKAN_CORE_INSTANCE_HPP
#define VULKAN_CORE_INSTANCE_HPP

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

VkInstance create_vulkan_instance
(
    const std::vector<const char*> &layers
);

void destroy_vulkan_instance
(
    VkInstance &vulkan_instance
);

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

class Vulkan_Instance
{

public:
    // Constructor.
    Vulkan_Instance
    (
        const std::vector<const char*> &layers
    );

    // Destructor.
    ~Vulkan_Instance();

    VkInstance get() const;

    // Prevent data duplication.
    Vulkan_Instance(const Vulkan_Instance&) = delete;
    Vulkan_Instance &operator = (const Vulkan_Instance&) = delete;

private:
    // We declare the members of the class to store.
    VkInstance vulkan_instance = VK_NULL_HANDLE;

};

#endif
