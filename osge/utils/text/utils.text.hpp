#ifndef UTILS_TEXT_HPP
#define UTILS_TEXT_HPP

#include <string>

namespace Text
{
    ////////////////////////////////
    ///// memory.addresses.cpp /////
    ////////////////////////////////

    template <typename any>

    std::string get_memory_address
    (
        const any &input
    );

    ////////////////////
    ///// trim.cpp /////
    ////////////////////

    std::string trim
    (
        const std::string &input
    );
}

#endif
