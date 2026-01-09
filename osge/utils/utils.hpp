#ifndef UTILS_HPP
#define UTILS_HPP

#include "files/utils.files.hpp"
#include "host/utils.host.hpp"
#include "logs/utils.logs.hpp"
#include "miscellaneous/utils.miscellaneous.hpp"
#include "parsers/utils.parsers.hpp"
#include "text/utils.text.hpp"

namespace Utils
{
    /////////////////////////////////
    ///// files/utils.files.hpp /////
    /////////////////////////////////

    namespace Files = ::Files;

    ///////////////////////////////
    ///// host/utils.host.hpp /////
    ///////////////////////////////

    namespace Host = ::Host;

    ///////////////////////////////
    ///// logs/utils.logs.hpp /////
    ///////////////////////////////

    namespace Logs = ::Logs;

    /////////////////////////////////////////////////
    ///// miscellaneous/utils.miscellaneous.hpp /////
    /////////////////////////////////////////////////

    namespace Miscellaneous = ::Miscellaneous;

    /////////////////////////////////////
    ///// parsers/utils.parsers.hpp /////
    /////////////////////////////////////

    namespace Parsers = ::Parsers;

    ///////////////////////////////
    ///// text/utils.text.hpp /////
    ///////////////////////////////

    namespace Text = ::Text;
}

#endif
