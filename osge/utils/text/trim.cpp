#include "utils.text.hpp"

#include <string>

/*
    Remove the starting and trailing whitespaces from some text.

    Tasks:
        1) Detect the "real" start of the input. Return an empty string if the input is empty itself.
        2) Detect the "real" end of the input.
        3) Remove all extra characters composing the "fake" start and end if existing.

    Parameters:
        - input / string / Text to trim.

    Returns:
        A string containing the trimmed text.
*/
std::string Text::trim
(
    const std::string &input
)
{
    const size_t real_start = input.find_first_not_of(" \t\r\n");

    if (real_start == std::string::npos)
        return "";

    const size_t real_end = input.find_last_not_of(" \t\r\n");
    const std::string output = input.substr(real_start, real_end - real_start + 1);

    return output;
}
