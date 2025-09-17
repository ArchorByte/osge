cmake_minimum_required(VERSION 3.10)
project(new_osge_project)
set(CMAKE_CXX_STANDARD 17)

# Include the headers in the "./include" folder.
include_directories (
    ${CMAKE_SOURCE_DIR}/include
)

# Windows systems.
if (WIN32)
    set(WINDOWS_BINARIES ${CMAKE_SOURCE_DIR}/bin/windows)

    # SDL3 headers and binaries.
    set(SDL3_LIB ${WINDOWS_BINARIES}/SDL3.lib)
    include_directories(${CMAKE_SOURCE_DIR}/include/SDL3)

    # Vulkan headers and binaries.
    set(VULKAN_LIB ${WINDOWS_BINARIES}/vulkan-1.lib)
    include_directories(${CMAKE_SOURCE_DIR}/include/vulkan)

# Linux systems.
else()
    set(LINUX_BINARIES ${CMAKE_SOURCE_DIR}/bin/linux)

    # SDL3 headers and binaries.
    set(SDL3_LIB ${LINUX_BINARIES}/libSDL3.so)
    include_directories(${CMAKE_SOURCE_DIR}/include/SDL3)

    # Vulkan headers and binaries.
    set(VULKAN_LIB ${LINUX_BINARIES}/libvulkan.so)
    include_directories(${CMAKE_SOURCE_DIR}/include/vulkan)

endif()

# Include folders for compilation.
include_directories (
    ${CMAKE_SOURCE_DIR}/environment
    ${CMAKE_SOURCE_DIR}/game
    ${CMAKE_SOURCE_DIR}/game/engine
    ${CMAKE_SOURCE_DIR}/logs
    ${CMAKE_SOURCE_DIR}/opengl
    ${CMAKE_SOURCE_DIR}/sdl3
    ${CMAKE_SOURCE_DIR}/helpers
    ${CMAKE_SOURCE_DIR}/vulkan
    ${CMAKE_SOURCE_DIR}/vulkan/buffers
    ${CMAKE_SOURCE_DIR}/vulkan/commands
    ${CMAKE_SOURCE_DIR}/vulkan/core
    ${CMAKE_SOURCE_DIR}/vulkan/descriptors
    ${CMAKE_SOURCE_DIR}/vulkan/device
    ${CMAKE_SOURCE_DIR}/vulkan/pipeline
    ${CMAKE_SOURCE_DIR}/vulkan/queues
    ${CMAKE_SOURCE_DIR}/vulkan/render
    ${CMAKE_SOURCE_DIR}/vulkan/render/sync
    ${CMAKE_SOURCE_DIR}/vulkan/shaders
    ${CMAKE_SOURCE_DIR}/vulkan/swapchain
    ${CMAKE_SOURCE_DIR}/vulkan/textures
    ${CMAKE_SOURCE_DIR}/vulkan/uniform
    ${CMAKE_SOURCE_DIR}/vulkan/helpers
    ${CMAKE_SOURCE_DIR}/vulkan/vertex
)

# Select all C++ scripts contained in the previously included folder.
file(GLOB MAIN main.cpp)
file(GLOB ENV environment/*.cpp)
file(GLOB GAME game/*.cpp)
file(GLOB GAME_ENGINE game/engine/*.cpp)
file(GLOB LOGS logs/*.cpp)
file(GLOB OPENGL opengl/*.cpp)
file(GLOB SDL3 sdl3/*.cpp)
file(GLOB HELPERS helpers/*.cpp)
file(GLOB VULKAN vulkan/*.cpp)
file(GLOB VULKAN_BUFFERS vulkan/buffers/*.cpp)
file(GLOB VULKAN_COMMANDS vulkan/commands/*.cpp)
file(GLOB VULKAN_CORE vulkan/core/*.cpp)
file(GLOB VULKAN_DESCRIPTORS vulkan/descriptors/*.cpp)
file(GLOB VULKAN_DEVICE vulkan/device/*.cpp)
file(GLOB VULKAN_PIPELINE vulkan/pipeline/*.cpp)
file(GLOB VULKAN_QUEUES vulkan/queues/*.cpp)
file(GLOB VULKAN_RENDER vulkan/render/*.cpp)
file(GLOB VULKAN_RENDER_SYNC vulkan/render/sync/*.cpp)
file(GLOB VULKAN_SHADERS vulkan/shaders/*.cpp)
file(GLOB VULKAN_SWAPCHAIN vulkan/swapchain/*.cpp)
file(GLOB VULKAN_TEXTURES vulkan/textures/*.cpp)
file(GLOB VULKAN_UNIFORM vulkan/uniform/*.cpp)
file(GLOB VULKAN_HELPERS vulkan/helpers/*.cpp)
file(GLOB VULKAN_VERTEX vulkan/vertex/*.cpp)

# Set a list with all of the sources we retrieved.
set(ALL_SOURCES
    ${MAIN}
    ${ENV}
    ${GAME}
    ${GAME_ENGINE}
    ${LOGS}
    ${OPENGL}
    ${SDL3}
    ${HELPERS}
    ${VULKAN}
    ${VULKAN_BUFFERS}
    ${VULKAN_COMMANDS}
    ${VULKAN_CORE}
    ${VULKAN_DESCRIPTORS}
    ${VULKAN_DEVICE}
    ${VULKAN_PIPELINE}
    ${VULKAN_QUEUES}
    ${VULKAN_RENDER}
    ${VULKAN_RENDER_SYNC}
    ${VULKAN_SHADERS}
    ${VULKAN_SWAPCHAIN}
    ${VULKAN_TEXTURES}
    ${VULKAN_UNIFORM}
    ${VULKAN_HELPERS}
    ${VULKAN_VERTEX}
)

# Add the all of the sources to the executable.
add_executable(new_osge_project ${ALL_SOURCES})

# Windows systems.
if (WIN32)
    # Copy the SDL3.dll file into the build folder.
    add_custom_command (
        TARGET new_osge_project POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/bin/windows/SDL3.dll"
        "${CMAKE_SOURCE_DIR}/build/win64/out"
    )

    # Copy the vulkan-1.dll file into the build folder.
    add_custom_command (
        TARGET new_osge_project POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/bin/windows/vulkan-1.dll"
        "${CMAKE_SOURCE_DIR}/build/win64/out"
    )

    # Link the .lib files to the executable.
    target_link_libraries(new_osge_project PRIVATE ${VULKAN_LIB} ${SDL3_LIB})

# Linux systems.
else()
    # Copy the libSDL3.so into the build folder.
    add_custom_command (
        TARGET new_osge_project POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/bin/linux/libSDL3.so"
        "${CMAKE_SOURCE_DIR}/build/linux/out/libSDL3.so"
    )

    # Copy the libvulkan.so file into the build folder.
    add_custom_command (
        TARGET new_osge_project POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/bin/linux/libvulkan.so"
        "${CMAKE_SOURCE_DIR}/build/linux/out/libvulkan.so"
    )

    # Link the .so files to the executable.
    target_link_libraries(new_osge_project PRIVATE ${VULKAN_LIB} ${SDL3_LIB})

endif()
