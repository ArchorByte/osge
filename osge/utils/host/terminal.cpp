#include "utils.host.hpp"

#if defined(_WIN64)
    #include "windows.h"
#endif

/*
    Hide the terminal for Windows systems.

    Tasks:
        1) Verify that we are running on Windows.
        2) Free the console.

    Parameters:
        No parameters.

    Returns:
        No object returned.
*/
void Host::hide_terminal()
{
    #if defined(_WIN64)
        FreeConsole();
    #endif
}
