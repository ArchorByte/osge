#include "help.versioning.hpp"

#include <string>

// Create a version string with an X.X.X.X format.
std::string create_version
(
    const int &version_variant,
    const int &version_major,
    const int &version_minor,
    const int &version_patch
)
{
    // Convert all of the parameters to string.
    const std::string version_variant_string = std::to_string(version_variant);
    const std::string version_major_string = std::to_string(version_major);
    const std::string version_minor_string = std::to_string(version_minor);
    const std::string version_patch_string = std::to_string(version_patch);

    // Format the version output.
    const std::string version = version_variant_string + "." + version_major_string + "." + version_minor_string + "." + version_patch_string;
    return version;
}
