#ifndef VULKAN_RENDERER_HPP
#define VULKAN_RENDERER_HPP

#include "buffers/vulkan.buffers.hpp"
#include "colors/vulkan.colors.hpp"
#include "core/vulkan.core.hpp"
#include "depth/vulkan.depth.hpp"
#include "descriptors/vulkan.descriptors.hpp"
#include "devices/vulkan.devices.hpp"
#include "images/vulkan.images.hpp"
#include "queues/vulkan.queues.hpp"
#include "shaders/vulkan.shaders.hpp"
#include "vertex/vulkan.vertex.hpp"

namespace Vulkan
{
    //////////////////////////////////////
    ///// buffers/vulkan.buffers.hpp /////
    //////////////////////////////////////

    namespace Buffers = ::Buffers;

    ////////////////////////////////////
    ///// colors/vulkan.colors.hpp /////
    ////////////////////////////////////

    namespace Colors = ::Colors;

    ////////////////////////////////
    ///// core/vulkan.core.hpp /////
    ////////////////////////////////

    namespace Core = ::Core;

    //////////////////////////////////
    ///// depth/vulkan.depth.hpp /////
    //////////////////////////////////

    namespace Depth = ::Depth;

    //////////////////////////////////////////////
    ///// descriptors/vulkan.descriptors.hpp /////
    //////////////////////////////////////////////

    namespace Descriptors = ::Descriptors;

    //////////////////////////////////////
    ///// devices/vulkan.devices.hpp /////
    //////////////////////////////////////

    namespace Devices = ::Devices;

    ////////////////////////////////////
    ///// images/vulkan.images.hpp /////
    ////////////////////////////////////

    namespace Images = ::Images;

    ////////////////////////////////////
    ///// queues/vulkan.queues.hpp /////
    ////////////////////////////////////

    namespace Queues = ::Queues;

    //////////////////////////////////////
    ///// shaders/vulkan.shaders.hpp /////
    //////////////////////////////////////

    namespace Shaders = ::Shaders;

    /////////////////////////////////////
    ///// shaders/vulkan.vertex.hpp /////
    /////////////////////////////////////

    namespace Vertex = ::Vertex;
}

#endif
