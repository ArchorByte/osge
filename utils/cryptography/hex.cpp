#include "hex.hpp"

#include <string>

// Encode an input using hex.
std::string hex_encode
(
    const unsigned char* data,
    const size_t &length
)
{
    static const char* hex_digits = "0123456789ABCDEF"; // Characters used for hexadecimal.
    std::string hex;

    for (int i = 0; i < length; i++)
    {
        const unsigned char character = data[i];

        hex += hex_digits[character >> 4];   // Select the high 4 bits from the hex alphabet.
        hex += hex_digits[character & 0x0F]; // Select the low 4 bits from the hex alphabet.
    }

    return hex;
}

// Decode an input using hex.
std::string hex_decode
(
    const std::string &input
)
{
    std::string output;

    for (int i = 0; i < input.length() / 2; i++)
    {
        const std::string hex = input.substr(i * 2, 2);                             // Select a pair of two characters (hex format).
        const char character = static_cast<char>(strtol(hex.c_str(), nullptr, 16)); // Convert that pair into a byte.

        output += character;
    }

    return output;
}
