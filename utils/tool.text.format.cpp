#include "tool.text.format.hpp"

#include <string>
#include <cctype>
#include <algorithm>

// Note: The force_string is in the tool.text.format.hpp script.

// Remove whitespaces from the start and end of the string.
std::string trim
(
    const std::string &input
)
{
    // Calculate the real start and end of the string by ignoring whitespaces.
    const size_t string_start = input.find_first_not_of(" \t\r\n");
    const size_t string_end = input.find_last_not_of(" \t\r\n");

    // Return an empty string if necessary or the trimmed text.
    const std::string output = string_start == std::string::npos ? "" : input.substr(string_start, string_end - string_start + 1);

    return output;
}
