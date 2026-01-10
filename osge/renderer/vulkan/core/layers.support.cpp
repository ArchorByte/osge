#include "vulkan.core.hpp"
#include "osge/utils/utils.hpp"
#include <cstring>
#include <libraries/vulkan/vulkan.h>
#include <string>
#include <vector>

/*
    Check if some requested layers are supported by the device.

    Tasks:
        1) Verify the parameters.
        2) Register all available layers.
        3) Verify the requested layers are within the supported layers list.

    Parameters:
        - layers / vector<const char*> / Requested layers to check.

    Returns:
        A boolean that either confirms the layers are supported or not.
*/
bool Vulkan::Core::check_layers_support
(
    const std::vector<const char*> &layers
)
{
    Utils::Logs::log("Verifying layers support..");

    if (layers.size() < 1)
    {
        Utils::Logs::log("The layers support verification succeeded! No layers to verify, skipped!");
        return true;
    }

    uint32_t layers_count = 0;
    vkEnumerateInstanceLayerProperties(&layers_count, nullptr);

    if (layers_count < 1)
    {
        Utils::Logs::error_log("The layers support verification has failed! No layers found!");
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
            Utils::Logs::error_log("The layers support verification has failed! The layer " + std::string(layer_name) + " is not supported by this device!");
            return false;
        }

        Utils::Logs::log("- Layer \"" + std::string(layer_name) + "\" supported!");
    }

    Utils::Logs::log("The layers support verification succeeded!");
    return true;
}
