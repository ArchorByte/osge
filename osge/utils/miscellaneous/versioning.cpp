#include "utils.miscellaneous.hpp"

#include <string>

/*
    Simple helper that makes a version in the X.X.X.X format.

    Tasks:
        1) Convert all parameters into string.
        2) Verify the results.
        3) Make the version.

    Parameters:
        - version_variant / int / First X of X.X.X.X.
        - version_major   / int / Second X.
        - version_minor   / int / Third X.
        - version_patch   / int / Fourth X.

    Returns:
        The created version number.
*/
std::string Miscellaneous::create_version
(
    const int &version_variant,
    const int &version_major,
    const int &version_minor,
    const int &version_patch
)
{
    std::string version_variant_string = std::to_string(version_variant);
    std::string version_major_string = std::to_string(version_major);
    std::string version_minor_string = std::to_string(version_minor);
    std::string version_patch_string = std::to_string(version_patch);

    if (version_variant_string.empty())
        version_variant_string = "0";

    if (version_major_string.empty())
        version_major_string = "0";

    if (version_minor_string.empty())
        version_minor_string = "0";

    if (version_patch_string.empty())
        version_patch_string = "0";

    const std::string version = version_variant_string + "." + version_major_string + "." + version_minor_string + "." + version_patch_string;
    return version;
}
