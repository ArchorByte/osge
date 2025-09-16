#include "config/engine.config.hpp"
#include "config/game.config.hpp"
#include "config/engine.version.hpp"
#include "logs/logs.popup.hpp"
#include "logs/logs.handler.hpp"
#include "sdl3/sdl3.instance.hpp"
#include "sdl3/window.handler.hpp"
#include "environment/env.console.hpp"
#include "environment/env.displays.hpp"
#include "environment/env.resolutions.hpp"
#include "environment/env.system.hpp"
#include "utils/tool.parser.hpp"
#include "utils/tool.integer.hpp"
#include "utils/tool.files.hpp"
#include "vulkan/vulkan.run.hpp"
#include "opengl/opengl.run.hpp"

#include <vulkan/vulkan.h>
#include <SDL3/SDL.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <map>
#include <filesystem>

// Handle the exceptions and trigger a crash only when we finished to clear memory and inform the player of the crash.
void crash_handler
(
    const std::string &error
)
{
    error_log("Note: Every object created by the engine has been destroyed before crashing to prevent any memory overload or memory leak.");
    open_crash_popup(error); // Open a message box to inform the player of the game crash.
    fatal_error_log(error);  // Finally trigger the crash.
}

int main()
{
    try
    {
        #ifdef ENABLE_LOGS_FILE
            if (std::filesystem::exists(LOGS_FILE_NAME)) std::filesystem::remove(LOGS_FILE_NAME); // We reset the logs file by deleting it if it already exists.
            create_new_empty_file(LOGS_FILE_NAME);
        #endif

        // We disable the console on Windows if we're in release mode.
        #ifndef DEBUG_MODE
            #if defined(_WIN64)
                hide_console();
            #endif
        #else
            log("WARNING: Running in debug mode!");
        #endif

        log("Running on OSGE v" + std::to_string(ENGINE_VERSION_VARIANT) + "." + std::to_string(ENGINE_VERSION_MAJOR) + "." + std::to_string(ENGINE_VERSION_MINOR) + "." + std::to_string(ENGINE_VERSION_PATCH) + ".");
        start_sdl3_instance();
        check_operating_system_support();                              // Check if we are running on a supported operating system.
        std::vector<int> display_indexes = get_available_displays(); // Retrieve all available monitors on this device.

        // We create a new default game config file if its missing.
        if (!std::filesystem::exists("game.config"))
        {
            create_new_empty_file("game.config");
            write_file("game.config", "WINDOW_WIDTH=1920\nWINDOW_HEIGHT=1080\nWINDOW_MODE=2\nGRAPHICS_API=0\nMONITOR=1\nGPU=1", false, true);
        }

        // Read the data in the config file and parse the data into a map.
        std::map<std::string, std::string> config = parse_config_file("game.config");

        // Read the config file content.
        std::string window_width = config["WINDOW_WIDTH"];
        std::string window_height = config["WINDOW_HEIGHT"];
        std::string window_mode = config["WINDOW_MODE"];
        std::string graphics_api = config["GRAPHICS_API"];
        std::string monitor = config["MONITOR"];

        // Note: If the graphics API provided is not handled, we use Vulkan by default.
        enum GraphicsAPI
        {
            VULKAN = 0,
            OPENGL = 1
        };

        if (!is_an_integer(window_mode))
        {
            error_log("Invalid window mode detected! Switched to full screen by default!");
            window_mode = "2";
        }

        if (!is_an_integer(graphics_api))
        {
            error_log("Invalid graphics API detected! Switched to Vulkan by default!");
            graphics_api = "0";
        }

        if (!is_an_integer(monitor))
        {
            error_log("Invalid monitor detected! Switched to the primary monitor by default!");
            monitor = "1";
        }

        int int_monitor = stoi(monitor);

        if (int_monitor < 1 || int_monitor > display_indexes.size())
        {
            error_log("Invalid monitor detected! Switched to the primary monitor by default!");
            int_monitor = 1;
        }

        // Retrieve the screen resolution.
        std::pair<int, int> screen_resolution = get_display_resolution(display_indexes[int_monitor - 1]);

        // List the "allowed" game resolutions.
        // Those resolutions are the 16/9 resolutions that can be used on this screen.
        std::vector<std::string> allowed_game_resolutions = get_allowed_game_resolutions(screen_resolution);

        // Select the window resolution at start.
        // We try to use the resolution in the config file.
        // If it's not a 16/9 resolution, we try to use the screen resolution.
        // If it's still not a 16/9 resolution, we select the highest allowed resolution.
        std::pair<int, int> game_resolution = select_game_resolution(config, screen_resolution, allowed_game_resolutions);

        // We create the main SDL3 window.
        SDL3_Window window(game_resolution.first, game_resolution.second, stoi(window_mode), std::string(GAME_TITLE), stoi(graphics_api));

        // Select the graphics API that we are going to use.
        switch (stoi(graphics_api))
        {
            case VULKAN:
                run_vulkan(window.get());
                break;

            case OPENGL:
                run_opengl();
                break;

            // If the graphics API provided by the config file is not valid, we use Vulkan by default.
            default:
                run_vulkan(window.get());
                break;
        }

        return 0;
    }
    catch (const std::exception &error) // Catch exceptions errors.
    {
        crash_handler(error.what());
    }
    catch (const std::string &error)    // Catch string errors.
    {
        crash_handler(error);
    }
    catch (const char &error)           // Catch char errors.
    {
        crash_handler(std::to_string(error));
    }
    catch (const char* &error)          // Catch char* errors.
    {
        crash_handler(std::string(error));
    }
    catch (const int &error)            // Catch integer errors.
    {
        crash_handler(std::to_string(error));
    }
}
