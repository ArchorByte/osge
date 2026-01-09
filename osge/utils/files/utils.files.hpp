#ifndef UTILS_FILES_HPP
#define UTILS_FILES_HPP

#include <string>
#include <vector>

namespace Files
{
    ////////////////////////
    ///// binaries.cpp /////
    ////////////////////////

    std::vector<char> read_binary_file
    (
        const std::string &file_path
    );

    ////////////////////////////
    ///// file.manager.cpp /////
    ////////////////////////////

    bool create_new_empty_file
    (
        const std::string &file_path
    );

    bool write_file
    (
        const bool &append,
        const std::string &data,
        const std::string &file_path
    );
}

#endif
