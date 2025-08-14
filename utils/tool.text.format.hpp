#include <string>
#include <sstream>

#ifndef TOOLS_TEXT_FORMAT_HPP
#define TOOLS_TEXT_FORMAT_HPP

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

std::string trim
(
    const std::string &input
);

///////////////////////////////////////////////////
//////////////////// Templates ////////////////////
///////////////////////////////////////////////////

// We validate any type as an input using a template.
template <typename any_type>

// Convert any input type into string.
std::string force_string
(
    any_type* input
)
{
    std::stringstream stream;
    std::string output;

    stream << static_cast<const void*>(input); // Insert the input into the string stream.
    output = stream.str(); // Get the output from the stream.

    return output;
};

#endif
