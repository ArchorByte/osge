#include "vulkan.pipeline.hpp"

#include "../../../utils/utils.hpp"
#include "libraries/vulkan/vulkan.h"

#include <cstdint>

/*
    Create an assembly state for a pipeline.

    Tasks:
        1) Make create info.
        2) Return it.

    Parameters:
        No parameters.

    Returns:
        The created assembly state create info.
*/
VkPipelineInputAssemblyStateCreateInfo create_assembly_state()
{
    Utils::Logs::log("Creating assembly state.. ", false);

    /*
        - sType                  / Defines the type of the structure.
        - topology               / Defines how vertex data is interpreted.
        - primitiveRestartEnable / Enables special index value to restart primitives.
    */
    const VkPipelineInputAssemblyStateCreateInfo create_info
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        .primitiveRestartEnable = VK_FALSE
    };

    Utils::Logs::log("Done!", true);
    return create_info;
}
