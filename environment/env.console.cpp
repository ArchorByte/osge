#include "env.console.hpp"

#include <iostream>

#if defined(_WIN64)
    #include "windows.h"
#endif

// Disable the console for Windows systems.
// We don't have any console problem on the other platforms.
void hide_console()
{
    #if defined(_WIN64)
        FreeConsole();
    #endif
}
