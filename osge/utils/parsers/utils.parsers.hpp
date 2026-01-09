#ifndef UTILS_PARSER_HPP
#define UTILS_PARSER_HPP

#include <map>
#include <string>

namespace Parsers
{
    /////////////////////////////
    ///// config.parser.cpp /////
    /////////////////////////////

    std::map<std::string, std::string> parse_config_file
    (
        const std::string &file_path
    );
}

#endif
