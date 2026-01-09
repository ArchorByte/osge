#include "utils.text.hpp"

#include <sstream>
#include <string>

template <typename any>

/*
    Get the memory address of any object in the hex format.

    Tasks:
        1) Create a string stream.
        2) Insert the input into it.
        3) Convert the stream into string.

    Parameters:
        - input / any (template) / Targeted object.

    Returns:
        A string containing the memory address in the hex format (example: 0x7ffd8683f640).
*/
std::string Text::get_memory_address
(
    const any &input
)
{
    std::stringstream stream;
    stream << static_cast<const void*>(&input);
    const std::string output = stream.str();

    return output;
};
