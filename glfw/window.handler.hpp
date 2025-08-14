#include <GLFW/glfw3.h>
#include <string>

#ifndef GLFW_WINDOW_HPP
#define GLFW_WINDOW_HPP

///////////////////////////////////////////////////
//////////////////// Functions ////////////////////
///////////////////////////////////////////////////

GLFWwindow* create_glfw_window
(
    int width,
    int height,
    int window_mode,
    std::string window_name,
    GLFWmonitor* monitor,
    int graphic_api
);

void destroy_window
(
    GLFWwindow* window
);

///////////////////////////////////////////////
//////////////////// Class ////////////////////
///////////////////////////////////////////////

class GLFW_Window
{

public:
    // Constructor.
    GLFW_Window
    (
        int width,
        int height,
        int window_mode,
        std::string window_name,
        GLFWmonitor* monitor,
        int graphic_api
    );

    // Destructor.
    ~GLFW_Window();

    GLFWwindow* get() const;

    // Prevent data duplication.
    GLFW_Window(const GLFW_Window&) = delete;
    GLFW_Window& operator=(const GLFW_Window&) = delete;

private:
    // We declare the members of the class to store.
    GLFWwindow* window;

};

#endif
