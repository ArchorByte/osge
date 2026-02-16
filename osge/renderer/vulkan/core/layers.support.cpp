#include "vulkan.core.hpp"

#include "libraries/vulkan/vulkan.h"
#include "osge/utils/utils.hpp"

#include <cstring>
#include <string>
#include <vector>

/*
    Check if some requested layers are supported by the device.

    Tasks:
        1) Verify function parameters.
        2) Register all available layers.
        3) Verify if the requested layers are within the supported layers list.

    Parameters:
        - layers / vector<const char*> / Layers to check.

    Returns:
        A boolean that either confirms the layers are supported or not.
*/
bool Core::check_layers_support
(
    const std::vector<const char*> &layers
)
{
    Utils::Logs::log("Verifying layers support.. ", false);

    if (layers.size() < 1)
    {
        Utils::Logs::log("Done!", true);
        return true;
    }

    uint32_t layers_count = 0;
    vkEnumerateInstanceLayerProperties(&layers_count, nullptr);

    if (layers_count < 1)
    {
        Utils::Logs::log("Failed! No layers were found.", true);
        return false;
    }

    std::vector<VkLayerProperties> available_layers(layers_count);
    vkEnumerateInstanceLayerProperties(&layers_count, available_layers.data());

    for (const char* layer_name : layers)
    {
        bool layer_found = false;

        for (const auto &layer_properties : available_layers)
        {
            if (strcmp(layer_name, layer_properties.layerName) == 0)
            {
                layer_found = true;
                break;
            }
        }

        if (!layer_found)
        {
            Utils::Logs::log("Failed! Layer " + std::string(layer_name) + " not supported.", true);
            return false;
        }
    }

    Utils::Logs::log("Done!", true);
    return true;
}
