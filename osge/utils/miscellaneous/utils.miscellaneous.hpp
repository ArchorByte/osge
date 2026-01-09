#ifndef UTILS_MISCELLANEOUS_HPP
#define UTILS_MISCELLANEOUS_HPP

#include <ctime>
#include <string>
#include <vector>

namespace Miscellaneous
{
    ////////////////////////
    ///// integers.cpp /////
    ////////////////////////

    bool is_an_integer
    (
        const std::string &input
    );

    ////////////////////
    ///// time.cpp /////
    ////////////////////

    time_t get_current_timestamp();

    std::vector<std::string> get_date_from_timestamp
    (
        const time_t &timestamp
    );

    //////////////////////////
    ///// versioning.cpp /////
    //////////////////////////

    std::string create_version
    (
        const int &version_variant,
        const int &version_major,
        const int &version_minor,
        const int &version_patch
    );
}

#endif
