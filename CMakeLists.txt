cmake_minimum_required(VERSION 3.10)
project(new_osge_project)
set(CMAKE_CXX_STANDARD 17)

# Include the headers.
include_directories (
    ${CMAKE_SOURCE_DIR}/include
)

# Windows systems.
if (WIN32)
    set(LIBS ${CMAKE_SOURCE_DIR}/bin/windows)

    # Get the LIB files in the Windows binaries folder to create the link to the DLL files.
    set(GLFW_LIBS ${LIBS}/glfw3.lib winmm gdi32 user32)
    set(VULKAN_LIBS ${LIBS}/vulkan-1.lib)

# Linux systems.
else()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

    include_directories(${GTK3_INCLUDE_DIRS})
    link_directories(${GTK3_LIBRARY_DIRS})
    add_definitions(${GTK3_CFLAGS_OTHER})

    set(SO ${CMAKE_SOURCE_DIR}/bin/linux)

    # Get the SO files in the Linux binaries folder.
    find_library(GLFW_LIBS glfw PATHS ${SO} NO_DEFAULT_PATH)
    find_library(VULKAN_LIBS vulkan PATHS ${SO} NO_DEFAULT_PATH)
    find_library(GTK3_LIBS gtk-3 PATHS ${SO} NO_DEFAULT_PATH)

endif()

# Folders to include in the compilation.
include_directories (
    ${CMAKE_SOURCE_DIR}/vulkan
    ${CMAKE_SOURCE_DIR}/vulkan/buffers
    ${CMAKE_SOURCE_DIR}/vulkan/command
    ${CMAKE_SOURCE_DIR}/vulkan/core
    ${CMAKE_SOURCE_DIR}/vulkan/device
    ${CMAKE_SOURCE_DIR}/vulkan/pipeline
    ${CMAKE_SOURCE_DIR}/vulkan/queues
    ${CMAKE_SOURCE_DIR}/vulkan/render
    ${CMAKE_SOURCE_DIR}/vulkan/render/sync
    ${CMAKE_SOURCE_DIR}/vulkan/shaders
    ${CMAKE_SOURCE_DIR}/vulkan/shaders/vertex
    ${CMAKE_SOURCE_DIR}/vulkan/swapchain
    ${CMAKE_SOURCE_DIR}/vulkan/textures
    ${CMAKE_SOURCE_DIR}/vulkan/uniform
    ${CMAKE_SOURCE_DIR}/vulkan/uniform/descriptor
    ${CMAKE_SOURCE_DIR}/vulkan/vertex
    ${CMAKE_SOURCE_DIR}/opengl
    ${CMAKE_SOURCE_DIR}/glfw
    ${CMAKE_SOURCE_DIR}/environment
    ${CMAKE_SOURCE_DIR}/tools
    ${CMAKE_SOURCE_DIR}/game
    ${CMAKE_SOURCE_DIR}/game/engine
    ${CMAKE_SOURCE_DIR}/logs
)

# Select all C++ scripts over the previously selected folders.
file(GLOB MAIN main.cpp)
file(GLOB VULKAN vulkan/*.cpp)
file(GLOB VULKAN_BUFFERS vulkan/buffers/*.cpp)
file(GLOB VULKAN_COMMAND vulkan/command/*.cpp)
file(GLOB VULKAN_CORE vulkan/core/*.cpp)
file(GLOB VULKAN_DEVICE vulkan/device/*.cpp)
file(GLOB VULKAN_PIPELINE vulkan/pipeline/*.cpp)
file(GLOB VULKAN_QUEUES vulkan/queues/*.cpp)
file(GLOB VULKAN_RENDER vulkan/render/*.cpp)
file(GLOB VULKAN_RENDER_SYNC vulkan/render/sync/*.cpp)
file(GLOB VULKAN_SHADERS vulkan/shaders/*.cpp)
file(GLOB VULKAN_SHADERS_VERTEX vulkan/shaders/vertex/*.cpp)
file(GLOB VULKAN_SWAPCHAIN vulkan/swapchain/*.cpp)
file(GLOB VULKAN_TEXTURES vulkan/textures/*.cpp)
file(GLOB VULKAN_UNIFORM vulkan/uniform/*.cpp)
file(GLOB VULKAN_UNIFORM_DESCRIPTOR vulkan/uniform/descriptor/*.cpp)
file(GLOB VULKAN_VERTEX vulkan/vertex/*.cpp)
file(GLOB OPENGL opengl/*.cpp)
file(GLOB GLFW glfw/*.cpp)
file(GLOB ENV environment/*.cpp)
file(GLOB TOOLS tools/*.cpp)
file(GLOB GAME game/*.cpp)
file(GLOB GAME_ENGINE game/engine/*.cpp)
file(GLOB LOGS logs/*.cpp)

# Take all sources for the executable.
set(ALL_SOURCES
    ${MAIN}
    ${VULKAN}
    ${VULKAN_BUFFERS}
    ${VULKAN_COMMAND}
    ${VULKAN_CORE}
    ${VULKAN_DEVICE}
    ${VULKAN_PIPELINE}
    ${VULKAN_QUEUES}
    ${VULKAN_RENDER}
    ${VULKAN_RENDER_SYNC}
    ${VULKAN_SHADERS}
    ${VULKAN_SHADERS_VERTEX}
    ${VULKAN_SWAPCHAIN}
    ${VULKAN_TEXTURES}
    ${VULKAN_UNIFORM}
    ${VULKAN_UNIFORM_DESCRIPTOR}
    ${VULKAN_VERTEX}
    ${OPENGL}
    ${GLFW}
    ${ENV}
    ${TOOLS}
    ${GAME}
    ${GAME_ENGINE}
    ${LOGS}
)

# Add the sources to the executable.
add_executable(new_osge_project ${ALL_SOURCES})

# Windows systems.
if (WIN32)
    # Copy the glfw3.dll file to the output folder.
    add_custom_command (
        TARGET new_osge_project POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/bin/windows/glfw3.dll"
        "${CMAKE_SOURCE_DIR}/build/win64/out"
    )

    # Copy the vulkan-1.dll file to the output folder.
    add_custom_command (
        TARGET new_osge_project POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/bin/windows/vulkan-1.dll"
        "${CMAKE_SOURCE_DIR}/build/win64/out"
    )

    # Link the LIB files to the executable.
    target_link_libraries(new_osge_project PRIVATE ${VULKAN_LIBS} ${GLFW_LIBS})

# Linux systems.
else()
    target_link_libraries(new_osge_project PRIVATE ${VULKAN_LIBS} ${GLFW_LIBS} ${GTK3_LIBRARIES})

    # Copy the libglfw.so file to the output folder.
    add_custom_command (
        TARGET new_osge_project POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/bin/linux/libglfw.so"
        "${CMAKE_SOURCE_DIR}/build/linux/out/libglfw.so"
    )

    # Copy the libvulkan.so file to the output folder.
    add_custom_command (
        TARGET new_osge_project POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/bin/linux/libvulkan.so"
        "${CMAKE_SOURCE_DIR}/build/linux/out/libvulkan.so"
    )

    # Copy the libgtk-3.so file to the output folder.
    add_custom_command (
        TARGET new_osge_project POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/bin/linux/libgtk-3.so"
        "${CMAKE_SOURCE_DIR}/build/linux/out/libgtk-3.so"
    )


endif()
