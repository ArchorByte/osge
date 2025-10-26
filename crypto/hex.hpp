#include <string>

#ifndef CRYPTO_HEX_HPP
#define CRYPTO_HEX_HPP

std::string hex_encode
(
    const unsigned char* data,
    const size_t &length
);

std::string hex_decode
(
    const std::string& hex
);

#endif
