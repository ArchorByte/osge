#include "validation.layers.hpp"

#include "../../logs/logs.handler.hpp"

#include <vulkan/vulkan.h>
#include <vector>
#include <cstring>
#include <string>
#include <cstdint>

// Check if some Vulkan validation layers are supported by the device.
void check_vulkan_validation_layers_compatibility
(
    const std::vector<const char*> &layers
)
{
    log("Verifying validation layers compatibility..");

    // Count the amount of layers supported by this device.
    uint32_t layers_count = 0;
    vkEnumerateInstanceLayerProperties(&layers_count, nullptr);

    if (layers.size() < 1)
    {
        log("No required validation layer provided! There is nothing to do.");
        log("The validation layers compatibility check up has ended successfully!");
        return;
    }

    // Register each layer supported by the device into a list.
    std::vector<VkLayerProperties> available_layers(layers_count);
    vkEnumerateInstanceLayerProperties(&layers_count, available_layers.data());

    // Analyze the compatibility for each given layer.
    for (const char* layer_name : layers)
    {
        std::string string_layer_name = layer_name;
        bool layer_found = false;

        // Check if this layer is available in the supported layers list.
        for (const auto &layer_properties : available_layers)
        {
            // Check if the layers name match.
            if (strcmp(layer_name, layer_properties.layerName) == 0)
            {
                layer_found = true;
                break;
            }
        }

        if (!layer_found)
        {
            error_log("The validation layer " + string_layer_name + " is not supported by this device!");
            fatal_error_log("The validation layers compatibility check up has failed!");
        }

        log("- Validation layer \"" + string_layer_name + "\" supported!");
    }

    log("The validation layers compatibility check up has ended successfully!");
}
