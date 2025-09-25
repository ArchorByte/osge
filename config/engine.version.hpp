#ifdef ENGINE_VERSION_HPP
#    undef ENGINE_VERSION_HPP
#endif

#define ENGINE_VERSION_HPP

namespace EngineVersion
{

// OSGE version manager.
// There is nothing to change here except if you updated the engine.
// Track the updates at: https://github.com/ArchorByte/osge.
constexpr int ENGINE_VERSION_VARIANT = 1;
constexpr int ENGINE_VERSION_MAJOR = 0;
constexpr int ENGINE_VERSION_MINOR = 0;
constexpr int ENGINE_VERSION_PATCH = 0;

}
