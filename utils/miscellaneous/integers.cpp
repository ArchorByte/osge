#include "utils.hpp"

#include <regex>
#include <string>

// Check if the input could be converted to integer safely.
bool is_an_integer
(
    const std::string &input
)
{
    static const std::regex pattern(R"(^-?\d+$)");
    const bool is_an_integer = std::regex_match(input, pattern);

    return is_an_integer;
}
