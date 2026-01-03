#include "env.console.hpp"

#include <iostream>

#if defined(_WIN64)
    #include "windows.h"
#endif

// Get rid of the console on Windows.
void hide_console()
{
    #if defined(_WIN64)
        FreeConsole();
    #endif
}
