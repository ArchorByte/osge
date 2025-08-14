#include "framerate.hpp"
#include "../logs/terminal.hpp"

#include <GLFW/glfw3.h>

auto previous_time = glfwGetTime();
int frames_count = 0;
int framerate = 0;

int framerate_counter()
{
    auto current_time = glfwGetTime();
    frames_count++;

    // If one second passed, we update the counter.
    if (current_time - previous_time >= 1.0f)
    {
        framerate = frames_count;      // We update the previous count to return the same frames count for the next ms.
        frames_count = 0;              // Reset the amount of frames in the counter.
        previous_time = current_time;  // Update the previous time with the current time.
    }

    // You can do something here with the framerate.
    return framerate;
}
