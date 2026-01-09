#include "utils.miscellaneous.hpp"

#include <regex>
#include <string>

/*
    Check if a string input can be converted to an integer.

    Tasks:
        1) Using regex, we create a pattern that detects any non-numeric characters.
        2) Apply the pattern to the input.

    Parameters:
        - input / string / Input to check.

    Returns:
        A boolean confirming whether the input is an integer or not.
*/
bool Miscellaneous::is_an_integer
(
    const std::string &input
)
{
    static const std::regex pattern(R"(^-?\d+$)");
    const bool is_an_integer = std::regex_match(input, pattern);

    return is_an_integer;
}
