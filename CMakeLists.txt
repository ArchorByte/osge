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

    # SDL3 .lib binaries.
    set(SDL3_LIB ${WINDOWS_BINARIES}/lib/SDL3.lib)

    # Vulkan .lib binaries.
    set(VULKAN_LIB ${WINDOWS_BINARIES}/lib/vulkan-1.lib)

    # OpenSSL .lib binaries.
    set(OPENSSL_CRYPTO_LIB ${WINDOWS_BINARIES}/lib/libcrypto.lib)
    set(OPENSSL_SSL_LIB ${WINDOWS_BINARIES}/lib/libssl.lib)

    # Copy the .dll binaries into the build folder.
    add_custom_target(copy_binaries ALL
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/bin/windows/dll/SDL3.dll" "${CMAKE_SOURCE_DIR}/build/win64/out"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/bin/windows/dll/vulkan-1.dll" "${CMAKE_SOURCE_DIR}/build/win64/out"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/bin/windows/dll/libcrypto-3-x64.dll" "${CMAKE_SOURCE_DIR}/build/win64/out"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/bin/windows/dll/libssl-3-x64.dll" "${CMAKE_SOURCE_DIR}/build/win64/out"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/bin/windows/dll/libgcc_s_seh-1.dll" "${CMAKE_SOURCE_DIR}/build/win64/out"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/bin/windows/dll/libstdc++-6.dll" "${CMAKE_SOURCE_DIR}/build/win64/out"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/bin/windows/dll/libwinpthread-1.dll" "${CMAKE_SOURCE_DIR}/build/win64/out"
    )

# Linux systems.
else()
    set(LINUX_BINARIES ${CMAKE_SOURCE_DIR}/bin/linux)

    # SDL3 binaries.
    set(SDL3_LIB ${LINUX_BINARIES}/libSDL3.so.0.2.24)

    # Vulkan binaries.
    set(VULKAN_LIB ${LINUX_BINARIES}/libvulkan.so.1.4.328)

    # OpenSSL binaries.
    set(OPENSSL_CRYPTO_LIB ${LINUX_BINARIES}/libcrypto.so.3.5.4)
    set(OPENSSL_SSL_LIB ${LINUX_BINARIES}/libssl.so.3.5.4)

    # Copy the .so binaries into the build folder.
    add_custom_target(copy_binaries ALL
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/bin/linux/libSDL3.so.0.2.24" "${CMAKE_SOURCE_DIR}/build/linux/out/libSDL3.so.0.2.24"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/bin/linux/libvulkan.so.1.4.328" "${CMAKE_SOURCE_DIR}/build/linux/out/libvulkan.so.1.4.328"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/bin/linux/libcrypto.so.3.5.4" "${CMAKE_SOURCE_DIR}/build/linux/out/libcrypto.so.3.5.4"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/bin/linux/libssl.so.3.5.4" "${CMAKE_SOURCE_DIR}/build/linux/out/libssl.so.3.5.4"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/bin/linux/libgcc_s-15-20251211.so.1" "${CMAKE_SOURCE_DIR}/build/linux/out/libgcc_s-15-20251211.so.1"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/bin/linux/libpthread.so.0" "${CMAKE_SOURCE_DIR}/build/linux/out/libpthread.so.0"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/bin/linux/libstdc++.so.6.0.34" "${CMAKE_SOURCE_DIR}/build/linux/out/libstdc++.so.6.0.34"
    )

    # Set the libraries location to the build folder.
    set(CMAKE_INSTALL_RPATH "$ORIGIN/out")
    set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)

endif()

# Library headers.
include_directories(${CMAKE_SOURCE_DIR}/include/SDL3)
include_directories(${CMAKE_SOURCE_DIR}/include/vulkan)
include_directories(${CMAKE_SOURCE_DIR}/include/openssl)

# Include folders for compilation.
include_directories (
    ${CMAKE_SOURCE_DIR}/crypto
    ${CMAKE_SOURCE_DIR}/environment
    ${CMAKE_SOURCE_DIR}/game
    ${CMAKE_SOURCE_DIR}/game/engine
    ${CMAKE_SOURCE_DIR}/logs
    ${CMAKE_SOURCE_DIR}/opengl
    ${CMAKE_SOURCE_DIR}/sdl3
    ${CMAKE_SOURCE_DIR}/utils
    ${CMAKE_SOURCE_DIR}/vulkan
    ${CMAKE_SOURCE_DIR}/vulkan/buffers
    ${CMAKE_SOURCE_DIR}/vulkan/colors
    ${CMAKE_SOURCE_DIR}/vulkan/commands
    ${CMAKE_SOURCE_DIR}/vulkan/core
    ${CMAKE_SOURCE_DIR}/vulkan/depth
    ${CMAKE_SOURCE_DIR}/vulkan/descriptors
    ${CMAKE_SOURCE_DIR}/vulkan/device
    ${CMAKE_SOURCE_DIR}/vulkan/utils
    ${CMAKE_SOURCE_DIR}/vulkan/images
    ${CMAKE_SOURCE_DIR}/vulkan/pipeline
    ${CMAKE_SOURCE_DIR}/vulkan/queues
    ${CMAKE_SOURCE_DIR}/vulkan/render
    ${CMAKE_SOURCE_DIR}/vulkan/render/sync
    ${CMAKE_SOURCE_DIR}/vulkan/shaders
    ${CMAKE_SOURCE_DIR}/vulkan/swapchain
    ${CMAKE_SOURCE_DIR}/vulkan/textures
    ${CMAKE_SOURCE_DIR}/vulkan/uniform
    ${CMAKE_SOURCE_DIR}/vulkan/vertex
    ${CMAKE_SOURCE_DIR}/vulkan/vertex/models
)

# Select all C++ scripts contained in the previously included folder.
file(GLOB MAIN main.cpp)
file(GLOB CRYPTO crypto/*.cpp)
file(GLOB ENV environment/*.cpp)
file(GLOB GAME game/*.cpp)
file(GLOB GAME_ENGINE game/engine/*.cpp)
file(GLOB LOGS logs/*.cpp)
file(GLOB OPENGL opengl/*.cpp)
file(GLOB SDL3 sdl3/*.cpp)
file(GLOB HELPERS utils/*.cpp)
file(GLOB VULKAN vulkan/*.cpp)
file(GLOB VULKAN_BUFFERS vulkan/buffers/*.cpp)
file(GLOB VULKAN_COLORS vulkan/colors/*.cpp)
file(GLOB VULKAN_COMMANDS vulkan/commands/*.cpp)
file(GLOB VULKAN_CORE vulkan/core/*.cpp)
file(GLOB VULKAN_DEPTH vulkan/depth/*.cpp)
file(GLOB VULKAN_DESCRIPTORS vulkan/descriptors/*.cpp)
file(GLOB VULKAN_DEVICE vulkan/device/*.cpp)
file(GLOB VULKAN_HELPERS vulkan/utils/*.cpp)
file(GLOB VULKAN_IMAGES vulkan/images/*.cpp)
file(GLOB VULKAN_PIPELINE vulkan/pipeline/*.cpp)
file(GLOB VULKAN_QUEUES vulkan/queues/*.cpp)
file(GLOB VULKAN_RENDER vulkan/render/*.cpp)
file(GLOB VULKAN_RENDER_SYNC vulkan/render/sync/*.cpp)
file(GLOB VULKAN_SHADERS vulkan/shaders/*.cpp)
file(GLOB VULKAN_SWAPCHAIN vulkan/swapchain/*.cpp)
file(GLOB VULKAN_TEXTURES vulkan/textures/*.cpp)
file(GLOB VULKAN_UNIFORM vulkan/uniform/*.cpp)
file(GLOB VULKAN_VERTEX vulkan/vertex/*.cpp)
file(GLOB VULKAN_VERTEX_MODELS vulkan/vertex/models/*.cpp)

# Set a list with all of the sources we retrieved.
set(ALL_SOURCES
    ${MAIN}
    ${CRYPTO}
    ${ENV}
    ${GAME}
    ${GAME_ENGINE}
    ${LOGS}
    ${OPENGL}
    ${SDL3}
    ${HELPERS}
    ${VULKAN}
    ${VULKAN_BUFFERS}
    ${VULKAN_COLORS}
    ${VULKAN_COMMANDS}
    ${VULKAN_CORE}
    ${VULKAN_DEPTH}
    ${VULKAN_DESCRIPTORS}
    ${VULKAN_DEVICE}
    ${VULKAN_HELPERS}
    ${VULKAN_IMAGES}
    ${VULKAN_PIPELINE}
    ${VULKAN_QUEUES}
    ${VULKAN_RENDER}
    ${VULKAN_RENDER_SYNC}
    ${VULKAN_SHADERS}
    ${VULKAN_SWAPCHAIN}
    ${VULKAN_TEXTURES}
    ${VULKAN_UNIFORM}
    ${VULKAN_VERTEX}
    ${VULKAN_VERTEX_MODELS}
)

# Add all scripts and libraries to the executable.
add_executable(new_osge_project ${ALL_SOURCES})
target_link_libraries(new_osge_project PRIVATE ${VULKAN_LIB} ${SDL3_LIB} ${OPENSSL_CRYPTO_LIB} ${OPENSSL_SSL_LIB})
